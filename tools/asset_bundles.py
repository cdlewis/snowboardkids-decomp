#!/usr/bin/env python3
"""Semantic asset ownership, non-destructive migration, and exact ROM scattering."""
from __future__ import annotations

import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import re
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import yaml
from tools.course_surface_data_common import load_yaml, pack_course_surface_data
from tools.course_graphics_common import pack_course_model_resources, compression_metadata_from_manifest
from tools.course_sprite_table_common import pack_course_sprite_table
from tools.readable_assets import pack_asset
from tools.huffman_asset import compress_huffman_asset

ROOT = Path(__file__).resolve().parents[1]
LAYOUT = Path('assets/layout/segments.yaml')
ASSET_TYPES = ('sprite_table', 'tilemap', 'race_animation', 'scene_animation',
               'pointer_bank', 'sample_bank', 'embedded_model', 'music_sequence', 'replay')
FAMILIES = {'course_model_resources': 'course_model_resources', 'model_resources': 'course_model_resources',
            'course_surface_data': 'course_surface_data', 'course_sprite_table': 'course_sprite_tables',
            **{kind: 'readable' for kind in ASSET_TYPES}, 'course_display_list': 'course_display_lists'}
CHAR_RESOURCES = ['SLASH_MODEL_RESOURCES', 'WENDY_MODEL_RESOURCES', 'JAM_MODEL_RESOURCES', 'LINDA_MODEL_RESOURCES', 'TOMMY_MODEL_RESOURCES', 'NINJA_MODEL_RESOURCES']
CHAR_ANIMATIONS = ['SLASH_RACE_ANIMATIONS', 'WENDY_RACE_ANIMATIONS', 'JAM_RACE_ANIMATIONS', 'LINDA_RACE_ANIMATIONS', 'TOMMY_RACE_ANIMATIONS', 'NINJA_RACE_ANIMATIONS']
COURSES = ['big_snowman', 'sunset_rock', 'rookie_mountain', 'dizzy_land', 'quicksand_valley',
           'silver_mountain', 'night_highway', 'animal_land', 'ninja_land', 'grass_valley']


def dump(value):
    return yaml.dump(value, Dumper=getattr(yaml, 'CSafeDumper', yaml.SafeDumper), sort_keys=False).encode()


def safe_path(root, relative):
    path = (root / relative).resolve()
    if not path.is_relative_to(root.resolve()):
        raise ValueError(f'asset reference escapes workspace: {relative}')
    return path


def commands(path):
    source = re.sub(r'/\*.*?\*/|//[^\n]*', '', path.read_text(), flags=re.S)
    body = source[source.index('= {')+3:source.rindex('}')]
    pattern = r'\{\{\s*0x([0-9a-fA-F]{8}),\s*0x([0-9a-fA-F]{8})\s*\}\}\s*,?'
    if re.sub(pattern, '', body).strip():
        raise ValueError(f'{path}: expected exact Gfx word pairs')
    return bytes.fromhex(''.join(a+b for a, b in re.findall(pattern, body)))


def source_segments(root):
    config = load_yaml(root / 'snowboardkids.yaml')
    segments = [s for s in config['segments'] if isinstance(s, list) or 'start' in s]
    rows = []
    for s, next_s in zip(segments, segments[1:]):
        if not isinstance(s, dict) or s.get('type') not in FAMILIES:
            continue
        s = copy.deepcopy(s)
        s['end'] = next_s['start'] if isinstance(next_s, dict) else next_s[0]
        s['legacy'] = f"assets/{FAMILIES[s['type']]}/{s['name']}" + ('.c' if s['type'] == 'course_display_list' else '.yaml')
        rows.append(s)
    return config, rows


CHARACTER_NAMES = ('slash', 'wendy', 'jam', 'linda', 'tommy', 'ninja')


def owner(s):
    name = s['name']
    for course in COURSES:
        if name.lower().startswith(course + '_'):
            return f'assets/courses/{course}'
    if name in CHAR_RESOURCES + CHAR_ANIMATIONS:
        index = (CHAR_RESOURCES if name in CHAR_RESOURCES else CHAR_ANIMATIONS).index(name)
        return f'assets/characters/{CHARACTER_NAMES[index]}'
    if name == 'PAN_DISPLAY_LIST':
        return 'assets/models/items/pan'
    if name == 'SHARED_RACE_MODEL_RESOURCES':
        return 'assets/shared/race_models'
    if name == 'SNOWBOARD_MODELS':
        return 'assets/models/snowboards'
    fmt = s['type']
    family = {'sprite_table': 'sprites', 'tilemap': 'tilemaps', 'sample_bank': 'audio/samples',
              'pointer_bank': 'audio/banks', 'music_sequence': 'audio/music', 'replay': 'replays',
              'race_animation': 'animations/race', 'scene_animation': 'animations/scenes',
              'model_resources': 'models/unidentified', 'course_display_list': 'models/unidentified',
              'embedded_model': 'models/unidentified'}.get(fmt, 'other')
    return f'assets/{family}/{name.lstrip("_").lower()}'


def migrate(root, if_absent=False):
    if (root / LAYOUT).exists():
        if if_absent:
            from tools.asset_name_migration import migrate_names
            migrate_names(root)
            print('Existing bundles retained; no authoritative sources overwritten.')
            return
        raise ValueError('bundles already exist; migration refuses to overwrite edits')
    config, rows = source_segments(root)
    owners = {s['name']: owner(s) for s in rows}
    resources = [s for s in rows if s['type'] in ('model_resources', 'course_model_resources')]
    for s in rows:
        if s['type'] == 'course_display_list' and s['name'] != 'PAN_DISPLAY_LIST':
            bank = next((r for r in resources if r['graphics_start'] <= s['start'] < r['graphics_end']), None)
            if bank and bank['name'] != 'SHARED_RACE_MODEL_RESOURCES':
                owners[s['name']] = owners[bank['name']]
    planned, bundles, layout_rows, file_map = {}, {}, [], {}

    def add(path, data):
        if path in planned and planned[path] != data:
            raise ValueError(f'conflicting ownership: {path}')
        planned[path] = data

    def copy_reference(old, destination):
        old = safe_path(root, old)
        if old in file_map:
            return file_map[old]
        file_map[old] = destination
        add(destination, old.read_bytes())
        return destination

    for s in rows:
        base = Path(owners[s['name']])
        bundle = bundles.setdefault(str(base), dict(schema_version=1, id=str(base.relative_to('assets')),
                                                    sources=[], models=[], animations=[], previews=[]))
        staged = root / 'build/asset-staging' / Path(s['legacy']).relative_to('assets')
        legacy = staged if staged.exists() else root / s['legacy']
        target = base / ('geometry' if s['type'] == 'course_display_list' else 'sources') / legacy.name
        row = dict(name=s['name'], decoder=s['type'], rom_start=s['start'], rom_end=s['end'],
                   legacy=s['legacy'], source=str(target), bundle=str(base / 'asset.yaml'))
        if s['type'] == 'course_display_list':
            add(target, legacy.read_bytes())
        else:
            m = load_yaml(legacy)
            row['compression'] = m.pop('compression', None)

            def rewrite(value, key=''):
                if isinstance(value, dict):
                    return {k: rewrite(v, k) for k, v in value.items()}
                if isinstance(value, list):
                    return [rewrite(v, key) for v in value]
                if isinstance(value, str) and key in ('path', 'preview', 'wav') and (legacy.parent / value).is_file():
                    old = legacy.parent / value
                    category = 'textures' if old.suffix == '.png' else 'geometry' if old.suffix == '.c' else 'data'
                    if 'samples' in old.parts:
                        dest = Path('assets/shared/audio_samples') / old.name
                    else:
                        dest = base / category / s['name'].lstrip('_') / Path(value)
                    new = copy_reference(old, dest)
                    return os.path.relpath(new, target.parent)
                return value

            m = rewrite(m)
            # Vertex and palette records are separate editable sources, not buried
            # beside compression metadata in a ROM-bank manifest.
            for p in m.get('parts', []):
                if p['type'] in ('vertices', 'palette'):
                    fields = 'vertices' if p['type'] == 'vertices' else 'values'
                    dest = base / ('geometry' if fields == 'vertices' else 'textures') / f"{s['name']}_{p['offset']:06X}.yaml"
                    add(dest, dump({fields: p.pop(fields)}))
                    p['source'] = os.path.relpath(dest, target.parent)
            add(target, dump(m))
            if m['format'] in ('race_animation', 'scene_animation'):
                bundle['animations'].append(os.path.relpath(target, base))
        bundle['sources'].append(os.path.relpath(target, base))
        layout_rows.append(row)

    # The graphics address space is assembled from canonical sources, never ROM.
    for bank in resources:
        base = Path(owners[bank['name']])
        gfx = [s for s in rows if s['type'] == 'course_display_list' and
               bank['graphics_start'] <= s['start'] < bank['graphics_end']]
        common = dict(resource=bank['name'], graphics=[s['name'] for s in gfx],
                      graphics_start=bank['graphics_start'], graphics_end=bank['graphics_end'])
        if bank['name'] in CHAR_RESOURCES:
            roots = bank['root_offsets']
            for variant in range(2):
                bundles[str(base)]['models'].append(dict(name='normal' if not variant else 'ghost',
                    roots=roots[variant*13:(variant+1)*13], character_index=CHAR_RESOURCES.index(bank['name']), **common))
        else:
            # Every stored list is indexed, including props not reachable from the
            # main course root. Preview each root independently to avoid losing it.
            for s in gfx:
                staged = root / 'build/asset-staging' / Path(s['legacy']).relative_to('assets')
                body = commands(staged if staged.exists() else root / s['legacy'])
                starts = [0] + [p+8 for p in range(0, len(body)-8, 8) if body[p] == 0xB8]
                for start in starts:
                    name = 'pan' if s['name'] == 'PAN_DISPLAY_LIST' else f"{s['name'].lstrip('_').lower()}_{start:04x}"
                    dest = owners[s['name']] if bank['name'] == 'SHARED_RACE_MODEL_RESOURCES' else str(base)
                    bundles[dest]['models'].append(dict(name=name, roots=[s['start']-bank['graphics_start']+start], **common))
    for s in rows:
        if s['type'] == 'embedded_model':
            base = owners[s['name']]
            for offset in s['root_offsets']:
                bundles[base]['models'].append(dict(name=f'board_{offset:06x}', embedded=s['name'], roots=[offset]))
    for base, bundle in bundles.items():
        bundle['previews'] = [f"{m['name']}.glb" for m in bundle['models']]
        add(Path(base) / 'asset.yaml', dump(bundle))
    layout = dict(schema_version=1, rom_sha1=config['sha1'], segments=layout_rows)
    add(LAYOUT, dump(layout))
    add(Path('assets/index.yaml'), dump(dict(schema_version=1, bundles=[dict(id=b['id'], manifest=f'{p}/asset.yaml',
         models=[m['name'] for m in b['models']], animations=len(b['animations'])) for p, b in bundles.items()])))
    # Preflight every destination before writing anything. Old extracted sources
    # stay intact and are no longer build inputs once bundles.mk exists.
    for path in planned:
        if safe_path(root, path).exists():
            raise ValueError(f'migration destination exists: {path}; nothing was overwritten')
    for path, data in planned.items():
        dest = root / path
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(data)
    write_makefile(root, layout)
    index_models(root, layout)
    print(f'Migrated {len(rows)} ROM segments into {len(bundles)} bundles; legacy sources retained.')


def materialize(root, row):
    path = safe_path(root, row['source'])
    m = load_yaml(path)
    for section in m.get('sections', []):
        m.update(load_yaml(safe_path(root, path.parent / section['source'])))
    m['compression'] = row['compression']
    for p in m.get('parts', []):
        if 'source' in p:
            p.update(load_yaml(safe_path(root, path.parent / p['source'])))
    return m, path


def index_models(root, layout):
    """Discover executable roots, not arbitrary Splat boundaries or vertex data."""
    if layout.get('schema_version',1)>=2:
        raise ValueError('Semantic model catalog is authoritative; edit its manifests instead of rebuilding the old catalog.')
    from tools.course_graphics_common import trace_course_graphics
    _, specs = source_segments(root)
    rows = {r['name']:r for r in layout['segments']}
    bundles = {r['bundle']:load_yaml(root/r['bundle']) for r in layout['segments']}
    for bundle in bundles.values():
        bundle['models'] = []
    scroll_source = (root/'src/race/ui/race_ui_effects.c').read_text()
    scroll_body = re.search(r'gRaceCourseScrollingTextureInit\[[^]]+\]\s*=\s*\{(.*?)\};',scroll_source,re.S)[1]
    scroll_rows = [[int(v,0) for v in re.findall(r'0x[0-9A-Fa-f]+|\d+',line)]
                   for line in re.findall(r'\{([^{}]+)\}',scroll_body)]
    scroll_courses = ['quicksand_valley','quicksand_valley','night_highway','animal_land',
                      'dizzy_land','dizzy_land','sunset_rock','sunset_rock']
    effect_source = (root/'src/race/course/race_course_effects.c').read_text()
    water_layer_body = re.search(r'gCourseWaterLayerEntries\[\]\s*=\s*\{(.*?)\};',effect_source,re.S)[1]
    water_layers = [[int(v,0) for v in re.findall(r'0x[0-9A-Fa-f]+|\d+',line)]
                    for line in re.findall(r'\{([^{}]+)\}',water_layer_body)]
    for bank in specs:
        if bank['type'] not in ('model_resources','course_model_resources'):
            continue
        gfx = [s for s in specs if s['type']=='course_display_list' and bank['graphics_start']<=s['start']<bank['graphics_end']]
        data = b''.join(pack_segment(root,rows[s['name']],False) for s in gfx)
        common = dict(resource=bank['name'],graphics=[s['name'] for s in gfx],
                      graphics_start=bank['graphics_start'],graphics_end=bank['graphics_end'])
        destination = rows[bank['name']]['bundle']
        if bank['name'] in CHAR_RESOURCES:
            for variant in range(2):
                bundles[destination]['models'].append(dict(name='normal' if variant==0 else 'ghost',
                    roots=bank['root_offsets'][13*variant:13*(variant+1)],
                    character_index=CHAR_RESOURCES.index(bank['name']),**common))
            for animation in ('_215BE0','_5E34A0'):
                bundles[destination]['models'].append(dict(name='scene_'+animation.lstrip('_').lower(),
                    roots=bank['root_offsets'][:13],character_index=CHAR_RESOURCES.index(bank['name']),
                    scene_animation=animation,**common))
            continue
        candidates = set(bank['root_offsets'])
        bindings = {}
        for course,scroll in zip(scroll_courses,scroll_rows):
            if bank['name'].lower().startswith(course+'_'):
                offset = scroll[0]&0xFFFFFF
                candidates.add(offset)
                for node in trace_course_graphics(data,[offset]).display_lists:
                    bindings[node.offset] = dict(asset='RACE_EFFECT_SPRITES',entry=scroll[1],wrap=True)
        # Self-contained model lists start with a pipe sync and bounding vertices.
        candidates.update(i for i in range(0,len(data)-16,8)
                          if data[i:i+4]==bytes.fromhex('e7000000') and data[i+8]==4 and data[i+16]==0xBE)
        for offset in sorted(candidates):
            graph = trace_course_graphics(data,[offset])
            if not any(data[p] in (0xB1,0xBF,0xB5) for node in graph.display_lists for p in range(node.offset,node.end,8)):
                continue
            source = next(s for s in gfx if s['start'] <= bank['graphics_start']+offset < s['end'])
            name = 'pan' if bank['name']=='SHARED_RACE_MODEL_RESOURCES' and offset==0x2660 else f"{source['name'].lstrip('_').lower()}_{offset:06x}"
            target = rows[source['name']]['bundle'] if bank['name']=='SHARED_RACE_MODEL_RESOURCES' else destination
            model = dict(name=name,roots=[offset],**common)
            if offset in bindings:
                model['texture_binding'] = bindings[offset]
            bundles[target]['models'].append(model)
        for i,water_layer in enumerate(water_layers):
            # race_scene_setup.c instantiates water layer 0 in course 0 and layers
            # 1/2 in course 3, whose loader selects Grass Valley (not ROM order).
            course = 'big_snowman' if i==0 else 'grass_valley'
            if bank['name'].lower().startswith(course+'_'):
                # Retain existing preview IDs for editable asset-tree compatibility.
                bundles[destination]['models'].append(dict(name=f'billboard_{i:02d}',roots=[water_layer[1]&0xFFFFFF],
                    setup=water_layer[0],vertex_binding=dict(address=water_layer[2],count=water_layer[3]),
                    texture_binding=dict(asset='RACE_EFFECT_SPRITES',entry=water_layer[5],wrap=True),**common))
    for spec in specs:
        if spec['type'] == 'embedded_model':
            destination = rows[spec['name']]['bundle']
            for offset in spec['root_offsets']:
                if offset in (0x180,0x4B8,0x828,0x3A48,0x3BC0,0x3DB0):
                    count = load_yaml(root/rows['SNOWBOARD_TEXTURES']['source'])['entry_count']
                    for entry in range(count):
                        bundles[destination]['models'].append(dict(name=f'board_{offset:06x}_texture_{entry:02d}',
                            embedded=spec['name'],roots=[offset],texture_binding=dict(asset='SNOWBOARD_TEXTURES',entry=entry,wrap=False)))
                else:
                    bundles[destination]['models'].append(dict(name=f'board_{offset:06x}',embedded=spec['name'],roots=[offset]))
    for path,bundle in bundles.items():
        bundle['previews'] = [m['name']+'.glb' for m in bundle['models']]
        referenced = {name for model in bundle['models'] for name in
                      ([model['resource']] if 'resource' in model else []) +
                      ([model['scene_animation']] if 'scene_animation' in model else []) +
                      ([model['texture_binding']['asset']] if 'texture_binding' in model else [])}
        bundle['resources'] = {name:os.path.relpath(rows[name]['source'],Path(path).parent) for name in sorted(referenced)}
        (root/path).write_bytes(dump(bundle))
    organize_shared_resources(root,rows,bundles)
    write_makefile(root,layout)
    source_index = root/'assets/source_views/index.yaml'
    if source_index.exists():
        for entry in load_yaml(source_index)['assets']:
            kind = entry['type']
            family = 'text' if kind=='MenuGlyphScript' else 'audio/effects' if kind=='sound_effects' else 'shared/source'
            if entry['name']=='gRaceCourseSceneryFogPanelVertices':
                family = 'models/source'
            base = Path('assets')/family/entry['name']
            path = str(base/'asset.yaml')
            source_bundle = dict(schema_version=1,id=str(base.relative_to('assets')),authority='source',
                sources=[os.path.relpath(entry['source'],base)],models=[],animations=[],previews=[],source_symbol=entry['name'])
            if entry['name']=='gRaceCourseSceneryFogPanelVertices':
                source_bundle['models'] = [dict(name='fog_panel',source=entry['source'],
                    vertices=entry['name'],display_list='gRaceCourseSceneryFogPanelDisplayList')]
                source_bundle['previews'] = ['fog_panel.glb']
            elif 'preview' in entry:
                source_bundle['previews'] = [os.path.relpath(entry['preview'],base)]
            if kind=='Vtx' and not source_bundle['models']:
                source_bundle['classification'] = 'runtime sprite/effect geometry; see source renderer for texture and topology'
            bundles[path] = source_bundle
            (root/path).parent.mkdir(parents=True,exist_ok=True)
            (root/path).write_bytes(dump(source_bundle))
    (root/'assets/index.yaml').write_bytes(dump(dict(schema_version=1,bundles=[dict(id=b['id'],manifest=p,
        models=[m['name'] for m in b['models']],animations=len(b['animations'])) for p,b in sorted(bundles.items())])))


def organize_shared_resources(root, rows, bundles):
    """A uniquely consumed range belongs beside its model, not in a bank bucket."""
    from tools.course_graphics_common import trace_course_graphics, collect_course_texture_references
    row = rows.get('SHARED_RACE_MODEL_RESOURCES')
    if row is None:
        return
    original = load_yaml(root/row['source'])
    if original.get('ownership_version') == 1:
        return
    manifest,path = materialize(root,row)
    vertex_users,texture_users,palette_users = {},{},{}
    graphics = None
    for bundle_path,bundle in bundles.items():
        for model in bundle['models']:
            if model.get('resource') != row['name']:
                continue
            if graphics is None:
                graphics = b''.join(pack_segment(root,rows[name],False) for name in model['graphics'])
            graph = trace_course_graphics(graphics,model['roots'])
            owner = str(Path(bundle_path).parent)
            for ref in graph.vertex_references:
                if ref.segment==3:
                    for offset in range(ref.offset,ref.offset+ref.count*16,16):
                        vertex_users.setdefault(offset,set()).add(owner)
            textures,palettes = collect_course_texture_references(graphics,graph,manifest['decompressed_size'])
            for ref in textures:
                texture_users.setdefault(ref.offset,set()).add(owner)
            for ref in palettes:
                palette_users.setdefault(ref.offset,set()).add(owner)
    planned = {}
    shared = 'assets/shared/race_models'
    def owning(users,offset):
        found = users.get(offset,set())
        return next(iter(found)) if len(found)==1 else shared
    def record(part,field,values,owner):
        dest = Path(owner)/('geometry' if field=='vertices' else 'textures')/f"{row['name']}_{part['type']}_{part['offset']:06X}.yaml"
        planned[dest] = dump({field:values})
        result = {k:v for k,v in part.items() if k not in ('source',field)}
        result['source'] = os.path.relpath(dest,Path(row['source']).parent)
        return result
    parts = []
    for p in manifest['parts']:
        if p['type']=='vertices':
            vertices = p['vertices']
            start = 0
            while start<len(vertices):
                owner = owning(vertex_users,p['offset']+16*start)
                end = start+1
                while end<len(vertices) and owning(vertex_users,p['offset']+16*end)==owner:
                    end+=1
                part = dict(type='vertices',offset=p['offset']+16*start)
                parts.append(record(part,'vertices',vertices[start:end],owner))
                start = end
        elif p['type']=='palette':
            parts.append(record(p,'values',p['values'],owning(palette_users,p['offset'])))
        elif p['type']=='texture':
            owner = owning(texture_users,p['offset'])
            dest = Path(owner)/'textures'/row['name']/Path(p['path']).name
            planned[dest] = (path.parent/p['path']).read_bytes()
            p['path'] = os.path.relpath(dest,Path(row['source']).parent)
            parts.append(p)
        else:
            parts.append(p)
    for dest,data in planned.items():
        if (root/dest).exists() and (root/dest).read_bytes()!=data:
            raise ValueError(f'shared-resource ownership destination conflicts: {dest}')
    for dest,data in planned.items():
        (root/dest).parent.mkdir(parents=True,exist_ok=True)
        (root/dest).write_bytes(data)
    original.update(parts=parts,ownership_version=1)
    path.write_bytes(dump(original))


def pack_segment(root, row, compressed=True):
    if row['decoder'] == 'course_display_list':
        result = commands(safe_path(root, row['source']))
    else:
        m, path = materialize(root, row)
        # Accept pre-migration layout records without restoring a generic Splat type.
        if row['decoder'] in ASSET_TYPES or row['decoder'] == 'readable_asset':
            if not compressed:
                m['compression'] = 'none'
            result = pack_asset(m, path.parent)
        else:
            packer = {'course_surface_data': pack_course_surface_data, 'course_sprite_table': pack_course_sprite_table,
                      'model_resources': pack_course_model_resources, 'course_model_resources': pack_course_model_resources}[row['decoder']]
            result = packer(m) if row['decoder'] == 'course_surface_data' else packer(m, path.parent)
            if compressed:
                result = compress_huffman_asset(result, compression_metadata_from_manifest(m))
    if compressed and len(result) != row['rom_end']-row['rom_start']:
        raise ValueError(f"{row['name']}: packed size {len(result)} exceeds/changes fixed ROM allocation {row['rom_end']-row['rom_start']}")
    return result


def dependencies(root, row):
    result = [str(LAYOUT), row['source']]
    if row['decoder'] != 'course_display_list':
        path = root / row['source']
        def walk(value):
            if isinstance(value, dict):
                for k, v in value.items():
                    if k in ('source', 'path') and isinstance(v, str):
                        result.append(str(safe_path(root, path.parent / v).relative_to(root)))
                    else:
                        walk(v)
            elif isinstance(value, list):
                for v in value:
                    walk(v)
        walk(load_yaml(path))
    return sorted(set(result))


def write_makefile(root, layout):
    manifest_dependencies = [str(LAYOUT)]+[r['source'] for r in layout['segments'] if r['decoder']!='course_display_list']
    lines = ['# Generated bundle scatter rules. Original object paths preserve linker ABI.',
             'assets/layout/bundles.mk: '+' '.join(manifest_dependencies)+' tools/asset_bundles.py',
             '\t$(V)$(PYTHON) tools/asset_bundles.py make-rules']
    for row in layout['segments']:
        obj = '$(BUILD_DIR)/' + (row['object'] if 'object' in row else str(Path(row['legacy']).with_suffix('.o')))
        dep = ' '.join(dependencies(root, row))
        lines += [f'{obj}: {dep} $(wildcard tools/*assets.py tools/*asset*.py tools/course_*common.py)',
                  '\t@mkdir -p $(dir $@)',
                  f"\t$(V)$(PYTHON) tools/asset_bundles.py pack {row['name']} --out $(@:.o=.bin)"]
        if row['decoder'] == 'course_display_list':
            lines.append('\t$(V)$(AS) $(ASFLAGS) -no-pad-sections -o $@ $(@:.o=.s)')
        else:
            lines.append('\t$(V)$(LD) -r -b binary -o $@ $(@:.o=.bin)')
    (root / 'assets/layout/bundles.mk').write_text('\n'.join(lines)+'\n')


def verify(root, layout, rom):
    if hashlib.sha1(rom).hexdigest() != layout['rom_sha1']:
        raise ValueError('verification requires the configured original ROM')
    failures = []
    for row in layout['segments']:
        try:
            if pack_segment(root, row) != rom[row['rom_start']:row['rom_end']]:
                raise ValueError('bytes differ from original ROM')
        except (ValueError, OSError, KeyError) as exc:
            failures.append(f"{row['name']}: {exc}")
    print(f"{len(layout['segments'])-len(failures)} exact bundled segment matches; {len(failures)} failures")
    if failures:
        raise ValueError('\n'.join(failures))


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('action', choices=['migrate', 'pack', 'verify', 'previews', 'index-models', 'make-rules'])
    p.add_argument('name', nargs='?')
    p.add_argument('--root', type=Path, default=ROOT)
    p.add_argument('--out', type=Path)
    p.add_argument('--if-absent', action='store_true')
    a = p.parse_args()
    root = a.root.resolve()
    if a.action == 'migrate':
        migrate(root, a.if_absent)
        return
    layout = load_yaml(root / LAYOUT)
    if a.action == 'make-rules':
        write_makefile(root,layout)
    elif a.action == 'index-models':
        index_models(root,layout)
    elif a.action == 'verify':
        verify(root, layout, (root / 'snowboardkids.z64').read_bytes())
    elif a.action == 'previews':
        from tools.model_preview import export_all
        export_all(root, layout, a.name)
    else:
        row = next(r for r in layout['segments'] if r['name'] == a.name)
        if a.out is None:
            p.error('pack requires --out')
        data = pack_segment(root, row)
        a.out.parent.mkdir(parents=True, exist_ok=True)
        a.out.write_bytes(data)
        if row['decoder'] == 'course_display_list':
            a.out.with_suffix('.s').write_text(f'.section .data\n.balign 8\n.globl {a.name}\n{a.name}:\n.incbin "{a.out.resolve()}"\n')
        deps = dependencies(root,row)
        a.out.with_suffix('.d').write_text(f"{a.out.with_suffix('.o')}: {' '.join(deps)}\n"+
                                          ''.join(f'{p}:\n' for p in deps))


if __name__ == '__main__':
    try:
        main()
    except (ValueError, OSError, KeyError) as exc:
        sys.exit(str(exc))
