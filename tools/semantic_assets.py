#!/usr/bin/env python3
"""Upgrade canonical bundles to object ownership; never build from legacy copies."""
from __future__ import annotations

import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import sys
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.asset_bundles import ROOT, LAYOUT, CHARACTER_NAMES, ASSET_TYPES, dump, materialize, pack_segment, write_makefile
from tools.course_surface_data_common import load_yaml

LEGACY_DIRS = ('course_display_lists','course_model_resources','course_sprite_tables','course_surface_data','readable')


def numbers(body):
    body = re.sub(r'/\*.*?\*/|//[^\n]*','',body,flags=re.S)
    if re.sub(r'-?(?:0x[0-9A-Fa-f]+|\d+)|[\s,{}]','',body):
        raise ValueError('nonliteral initializer in semantic registry adapter')
    return [int(x,0) for x in re.findall(r'-?(?:0x[0-9A-Fa-f]+|\d+)',body)]


def initializer(source, symbol):
    found = re.search(r'\b'+re.escape(symbol)+r'\[[^]]*\]\s*=\s*\{(.*?)\};',source,re.S)
    if not found:
        raise ValueError(f'missing runtime initializer {symbol}')
    return found[1]


def signed32(value):
    return value-(1<<32) if value >= 0x80000000 else value


def validate_registry(root, registry):
    items = registry['items']
    for family,count in (('projectile',5),('action',6)):
        ids = [v['runtime_id'] for v in items.values() if v['family']==family]
        if sorted(ids)!=list(range(1,count+1)):
            raise ValueError(f'incomplete/duplicate {family} item IDs: {ids}')
    hud = (root/'src/race/ui/race_hud.c').read_text()
    shot = numbers(initializer(hud,'gRaceTimerTensDigitTileOffsets'))
    action = numbers(initializer(hud,'gRaceTimerOnesDigitTileIds'))
    for name,item in items.items():
        i = item['runtime_id']
        expected = list(range(shot[i],shot[i]+3)) if item['family']=='projectile' else [action[i]]
        if item['icons']!=expected:
            raise ValueError(f'{name}: icon registry differs from HUD table')
    dispatch = (root/'src/race/player/race_player_pickup_effects.c').read_text()
    for name,item in items.items():
        if item['renderer'] not in dispatch:
            raise ValueError(f'{name}: renderer no longer appears in item dispatch')
    if sorted(c['runtime_id'] for c in registry['courses'].values())!=list(range(10)):
        raise ValueError('course registry must cover all ten runtime course IDs')


def name_characters(root):
    renames = {f'character_{i:02d}': name for i,name in enumerate(CHARACTER_NAMES)
               if name != f'character_{i:02d}' and (root/'assets/characters'/f'character_{i:02d}').exists()}
    if not renames:
        return
    for name in renames.values():
        if (root/'assets/characters'/name).exists():
            raise ValueError(f'character destination already exists: {name}; sources untouched')
    backup_parent = root/'asset-backups'
    backup_parent.mkdir(exist_ok=True)
    backup = Path(tempfile.mkdtemp(prefix='character-names-',dir=backup_parent))
    stage = Path(tempfile.mkdtemp(prefix='character-names-',dir=root/'build'))
    shutil.copytree(root/'assets',stage/'assets')
    for old,new in renames.items():
        (stage/'assets/characters'/old).rename(stage/'assets/characters'/new)
    pattern = re.compile(r'\b('+'|'.join(renames)+r')\b')
    for path in (stage/'assets').rglob('*.yaml'):
        original = path.read_text()
        updated = pattern.sub(lambda m: renames[m[0]],original)
        if updated != original:
            path.write_text(updated)
    write_makefile(stage,load_yaml(stage/LAYOUT))
    os.rename(root/'assets',backup/'assets')
    try:
        os.rename(stage/'assets',root/'assets')
    except BaseException:
        os.rename(backup/'assets',root/'assets')
        raise
    print(f'Named character bundles; original tree retained at {backup}/assets')


def upgrade(root, registry):
    name_characters(root)
    layout = load_yaml(root/LAYOUT)
    changed = False
    for row in layout['segments']:
        if row['decoder'] == 'readable_asset':
            kind = load_yaml(root/row['source'])['format']
            if kind not in ASSET_TYPES:
                raise ValueError(f'unsupported asset segment type: {kind}')
            row['decoder'] = kind
            changed = True
    if changed:
        (root/LAYOUT).write_bytes(dump(layout))
        write_makefile(root,layout)
    if layout.get('schema_version',1)>=2:
        validate_structure(root)
        print('Semantic bundles already current; editable sources retained.')
        return
    validate_registry(root,registry)
    rows = {r['name']:r for r in layout['segments']}
    baseline = {name:pack_segment(root,row) for name,row in rows.items()}
    rom = (root/'snowboardkids.z64').read_bytes()
    # Preserve edits on both sides. A legacy-only edit must be resolved explicitly.
    for name,row in rows.items():
        old = root/row['legacy']
        if not old.exists():
            continue
        legacy_row = dict(row,source=row['legacy'])
        if old.suffix=='.yaml':
            legacy_row['compression'] = load_yaml(old)['compression']
        legacy_bytes = pack_segment(root,legacy_row)
        if legacy_bytes!=rom[row['rom_start']:row['rom_end']] and legacy_bytes!=baseline[name]:
            raise ValueError(f'{name}: legacy edits conflict with canonical bundle; resolve before migration')
    backup_parent = root/'asset-backups'
    backup_parent.mkdir(exist_ok=True)
    backup = Path(tempfile.mkdtemp(prefix='semantic-',dir=backup_parent))
    stage = Path(tempfile.mkdtemp(prefix='semantic-',dir=root/'build'))
    shutil.copytree(root/'assets',stage/'assets')
    # The staging tree is disposable; the current canonical tree is untouched
    # until every rebuilt segment equals its pre-migration bytes.
    migrate_tree(root,stage,layout,registry)
    for row in layout['segments']:
        if pack_segment(stage,row)!=baseline[row['name']]:
            raise ValueError(f"{row['name']}: migration changed packed bytes; original assets untouched")
    validate_structure(stage)
    os.rename(root/'assets',backup/'assets')
    try:
        os.rename(stage/'assets',root/'assets')
    except BaseException:
        os.rename(backup/'assets',root/'assets')
        raise
    write_makefile(root,layout)
    print(f"Upgraded all {len(rows)} segments, 11 items and 10 courses; exact pre-migration bytes retained.")
    print(f'Original sources and legacy snapshots are recoverable at {backup}/assets')


def migrate_tree(original_root, root, layout, registry):
    rows = {r['name']:r for r in layout['segments']}
    old_index = load_yaml(root/'assets/index.yaml')
    bundles = {b['manifest']:load_yaml(root/b['manifest']) for b in old_index['bundles']}
    definitions = {}
    for path,b in bundles.items():
        for m in b.get('models',[]):
            key = b['id']+'/'+m['name']
            definitions[key] = dict(bundle=path,model=m)
    moves = {}
    documents = {}
    # ROM-shaped records are private to the packing layout, for every family.
    for row in rows.values():
        old = Path(row['source'])
        if row['decoder']!='course_display_list':
            new = Path('assets/layout/records')/(row['name']+'.yaml')
        elif '/courses/' in row['source']:
            course = Path(row['bundle']).parent.name
            role = row['name'].lower().removeprefix(course+'_').removeprefix('course_')
            role = 'terrain' if role=='display_list' else role.removesuffix('_display_list')
            new = Path(row['bundle']).parent/'geometry'/(role+'.c')
        else:
            new = old
        if old!=new:
            moves[old] = new
        row['source'] = str(new)
    # Eliminate format/address directory nesting from course-owned files.
    for path,b in bundles.items():
        if not b['id'].startswith('courses/'):
            continue
        base = Path(path).parent
        for file in sorted((root/base).rglob('*')):
            rel = file.relative_to(root)
            if not file.is_file() or rel in moves or file.suffix=='.glb' or file.name=='asset.yaml':
                continue
            if file.suffix=='.png':
                category = 'sprites' if 'SPRITE' in str(rel) or 'image_' in file.name else 'textures'
                dest = base/category/file.name
            elif file.suffix=='.yaml' and '/geometry/' in str(rel):
                dest = base/'geometry'/('vertices_'+file.stem.rsplit('_',1)[-1]+'.yaml')
            elif file.suffix=='.yaml' and '/textures/' in str(rel):
                dest = base/'textures'/('palette_'+file.stem.rsplit('_',1)[-1]+'.yaml')
            else:
                continue
            if rel!=dest:
                moves[rel] = dest
    # Name every item first; the same image offset keeps exactly one owner even
    # when multiple item entries alias it or select alternative palettes.
    sprite_manifests = {name:load_yaml(root/next(Path(k) for k,v in moves.items() if str(v)==row['source']))
        for name,row in rows.items() if name in ('RACE_ITEM_SPRITES','SHARED_UI_SPRITES','RACE_EFFECT_SPRITES')}
    sprite_sources = {name:next(Path(k) for k,v in moves.items() if str(v)==rows[name]['source']) for name in sprite_manifests}
    uses = {}
    item_images = {}
    for name,item in registry['items'].items():
        refs = dict(item.get('sprites',{}))
        refs['SHARED_UI_SPRITES'] = item['icons']
        item_images[name] = []
        for bank,indices in refs.items():
            m = sprite_manifests[bank]
            for index in indices:
                entry = m['entries'][index]
                image = next(i for i in m['images'] if i['offset']==entry['image_offset'])
                old = Path(os.path.normpath(sprite_sources[bank].parent/image['path']))
                uses.setdefault(old,set()).add(name)
                item_images[name].append((bank,index,old,entry['palette_index']))
    for old,owners in uses.items():
        base = Path('assets/items')/next(iter(owners)) if len(owners)==1 else Path('assets/shared/item_sprites')
        bank = next(bank for bank,src in sprite_sources.items() if old.is_relative_to(src.parent.parent))
        moves[old] = base/'sprites'/(bank.lstrip('_').lower()+'_'+old.name)
    # Move uniquely owned pan/fan model resources, not just catalog entries.
    for item_name,item in registry['items'].items():
        roots = item.get('model_roots',[])
        for definition in definitions.values():
            model = definition['model']
            if model.get('resource')=='SHARED_RACE_MODEL_RESOURCES' and model['roots'][0] in roots:
                old_base = Path(definition['bundle']).parent
                new_base = Path('assets/items')/item_name
                for file in (root/old_base).rglob('*'):
                    if file.is_file() and file.suffix!='.glb' and file.name!='asset.yaml':
                        old = file.relative_to(root)
                        if old not in moves:  # records already moved under layout
                            moves[old] = new_base/old.relative_to(old_base)
    # Preflight names before touching even the staging copy.
    for row in rows.values():
        row['source'] = str(moves.get(Path(row['source']),Path(row['source'])))
    reverse = {}
    for old,new in moves.items():
        if new in reverse and (root/old).read_bytes()!=(root/reverse[new]).read_bytes():
            raise ValueError(f'conflicting semantic resource destination {new}')
        reverse[new] = old
        if (root/new).exists() and new not in moves and new!=old and (root/new).read_bytes()!=(root/old).read_bytes():
            raise ValueError(f'destination already contains different data: {new}')

    def rewrite(value, old_base, new_base):
        if isinstance(value,dict):
            return {k:rewrite(v,old_base,new_base) for k,v in value.items()}
        if isinstance(value,list):
            return [rewrite(v,old_base,new_base) for v in value]
        if isinstance(value,str) and len(value)<240 and '\n' not in value:
            # Rewrite only references to actual files, not arbitrary strings.
            absolute = Path(os.path.normpath(old_base/value))
            if absolute in moves or (root/absolute).is_file():
                return os.path.relpath(moves.get(absolute,absolute),new_base)
        return value
    for file in (root/'assets').rglob('*.yaml'):
        rel = file.relative_to(root)
        if any(rel.is_relative_to(Path('assets')/d) for d in LEGACY_DIRS):
            continue
        new = moves.get(rel,rel)
        documents[new] = rewrite(load_yaml(file),rel.parent,new.parent)
    for old,new in moves.items():
        (root/new).parent.mkdir(parents=True,exist_ok=True)
        if old.suffix!='.yaml':
            shutil.copyfile(root/old,root/new)
    for new,m in documents.items():
        (root/new).parent.mkdir(parents=True,exist_ok=True)
        (root/new).write_bytes(dump(m))
    for old,new in moves.items():
        if old!=new and old not in reverse:
            (root/old).unlink()
    # Collision and path data are editable object data, not segment manifests.
    for course in registry['courses']:
        row = rows[course.upper()+'_COURSE_SURFACE_DATA']
        record_path = root/row['source']
        m = load_yaml(record_path)
        mesh_path = Path('assets/courses')/course/'collision/mesh.yaml'
        paths_path = Path('assets/courses')/course/'paths/surfaces.yaml'
        for dest,keys in ((mesh_path,('coordinates','faces')),(paths_path,('surfaces',))):
            (root/dest).parent.mkdir(parents=True,exist_ok=True)
            (root/dest).write_bytes(dump({k:m.pop(k) for k in keys}))
        m['sections'] = [dict(source=os.path.relpath(p,record_path.relative_to(root).parent)) for p in (mesh_path,paths_path)]
        record_path.write_bytes(dump(m))
    # Internal preview definitions retain address-space details; public manifests
    # describe assets and placements only.
    final_bundles = {}
    final_defs = {}
    root_lookup = {}
    for key,d in definitions.items():
        m = d['model']
        if m.get('resource')=='SHARED_RACE_MODEL_RESOURCES':
            root_lookup[m['roots'][0]] = key
        old_b = bundles[d['bundle']]
        if old_b['id'].startswith('courses/'):
            course = old_b['id'].split('/')[1]
            name = m['name'].removeprefix(course+'_').removeprefix('course_')
            name = name.replace('display_list_', '').replace('_display_list', '')
            if name == f"{m['roots'][0]:06x}":
                name = 'terrain'
            path = Path('assets/courses')/course/'props'/name/'asset.yaml'
        else:
            path = Path(d['bundle'])
            name = m['name']
        for item_name,item in registry['items'].items():
            if m.get('resource')=='SHARED_RACE_MODEL_RESOURCES' and m['roots'][0] in item.get('model_roots',[]):
                path = Path('assets/items')/item_name/'asset.yaml'
                name = item_name if len(item['model_roots'])==1 else ('front' if m['roots'][0]==item['model_roots'][0] else 'back')
        b = final_bundles.setdefault(str(path),dict(schema_version=2,id=str(path.parent.relative_to('assets')),models=[],animations=[]))
        new_key = b['id']+'/'+name
        m['name'] = name
        final_defs[new_key] = dict(bundle=str(path),model=m)
        d['new_key'] = new_key
        b['models'].append(dict(name=name,preview=name+'.glb'))
        # Animation sources have moved into records; exporter reads layout refs.
        m['animation_assets'] = [Path(p).stem for p in old_b.get('animations',[])]
    shared_model = next(d['model'] for d in final_defs.values() if d['model'].get('resource')=='SHARED_RACE_MODEL_RESOURCES')
    def model_for_root(address):
        if address in root_lookup:
            return definitions[root_lookup[address]]['new_key']
        name = f'scenery_{address:06x}'
        path = Path('assets/models/scenery')/name/'asset.yaml'
        key = 'models/scenery/'+name+'/'+name
        m = copy.deepcopy(shared_model)
        m.update(name=name,roots=[address])
        final_defs[key] = dict(bundle=str(path),model=m)
        final_bundles[str(path)] = dict(schema_version=2,id='models/scenery/'+name,models=[dict(name=name,preview=name+'.glb')],animations=[])
        return key
    source = (original_root/'src/race/course/race_course_effects.c').read_text()
    pointers = re.findall(r'0x0200[\dA-Fa-f]{4}|gRaceCourseSceneryFogPanelDisplayList',initializer(source,'gRaceCourseObjectDisplayLists'))
    for course,spec in registry['courses'].items():
        base = Path('assets/courses')/course
        b = dict(schema_version=2,id='courses/'+course,models=[dict(name='preview',preview='preview.glb')],animations=[],
                 collision='collision/mesh.yaml',paths='paths/surfaces.yaml',scenery=[],effects=[])
        components = []
        for key,d in definitions.items():
            if not key.startswith('courses/'+course+'/'):
                continue
            name = key.rsplit('/',1)[1]
            new_key = d['new_key']
            if name.startswith(course+'_course_display_list_'):
                b['terrain'] = new_key
                components.append(dict(model=new_key))
            elif '_backdrop_display_list_' in name:
                b['backdrop'] = new_key
                components.append(dict(model=new_key))
            elif 'billboard_' in name or ('scrolling_texture' in name and '_geometry' not in name):
                b['effects'].append(new_key)
                components.append(dict(model=new_key))
        if 'terrain' not in b:
            raise ValueError(f'{course}: no main terrain model')
        placement_values = numbers(initializer(source,spec['scenery']))
        if len(placement_values)%7:
            raise ValueError('invalid scenery stride')
        for i in range(0,len(placement_values),7):
            model_id,unused,x,y,z,angle,padding = placement_values[i:i+7]
            if model_id==-1:
                break
            pointer = pointers[model_id]
            if pointer.startswith('0x'):
                key = model_for_root(int(pointer,16)&0xFFFFFF)
            else:
                key = next(k for k in final_defs if k.endswith('/fog_panel'))
            placement = dict(model=key,position=[signed32(v)/65536 for v in (x,y,z)],rotation_y=angle)
            b['scenery'].append(placement)
            components.append(placement)
        b['scenery_source'] = dict(file='src/race/course/race_course_effects.c',symbol=spec['scenery'])
        final_bundles[str(base/'asset.yaml')] = b
        final_defs[b['id']+'/preview'] = dict(bundle=str(base/'asset.yaml'),model=dict(name='preview',components=components))
    for name,item in registry['items'].items():
        base = Path('assets/items')/name
        b = final_bundles.setdefault(str(base/'asset.yaml'),dict(schema_version=2,id='items/'+name,models=[],animations=[]))
        b.update(runtime=dict(family=item['family'],id=item['runtime_id']),renderer=item['renderer'],icons=[],sprites=[],
                 representation='model' if b['models'] else 'procedural' if 'procedural' in item else 'sprites')
        if 'procedural' in item:
            b['effect'] = item['procedural']
        if 'animation_source' in item:
            b['animation_source'] = item['animation_source']
        for bank,index,old,palette in item_images[name]:
            ref = dict(image=os.path.relpath(moves.get(old,old),base),palette=dict(bank=bank,index=palette),entry=index)
            b['icons' if bank=='SHARED_UI_SPRITES' else 'sprites'].append(ref)
        b['shared_effects'] = ['src/race/items/race_item_effects.c','src/race/player/race_player_pickup_effects.c']
    # Retain non-model family bundles with corrected file references, but hide
    # packing records from their public sources lists.
    for path,old in bundles.items():
        if path in final_bundles or old.get('models') or old['id'].startswith('courses/'):
            continue
        b = documents.get(Path(path),old)
        b['schema_version'] = 2
        b.pop('resources',None)
        b['sources'] = [p for p in b.get('sources',[]) if '/layout/' not in str(Path(path).parent/p)]
        final_bundles[path] = b
    # Supply a precise local-file inventory instead of bank-format manifests.
    for path,b in final_bundles.items():
        base = Path(path).parent
        b['files'] = [str(p.relative_to(root/base)) for p in sorted((root/base).rglob('*'))
                      if p.is_file() and p.suffix!='.glb' and p.name!='asset.yaml' and '/sources/' not in str(p)]
        (root/path).parent.mkdir(parents=True,exist_ok=True)
        (root/path).write_bytes(dump(b))
    # Remove stale generated manifests/previews and empty containers only inside
    # this disposable staging tree. The untouched tree is atomically backed up.
    retired = root/'retired'
    for file in list((root/'assets').rglob('asset.yaml'))+list((root/'assets').rglob('*.glb')):
        rel = file.relative_to(root)
        if file.suffix=='.glb' or str(rel) not in final_bundles:
            dest = retired/rel
            dest.parent.mkdir(parents=True,exist_ok=True)
            shutil.move(file,dest)
    for directory in LEGACY_DIRS:
        path = root/'assets'/directory
        if path.exists():
            dest = retired/'legacy'/directory
            dest.parent.mkdir(parents=True,exist_ok=True)
            shutil.move(path,dest)
    for path in sorted((root/'assets').rglob('*'),key=lambda p:len(p.parts),reverse=True):
        if path.is_dir() and not any(path.iterdir()):
            path.rmdir()
    for row in rows.values():
        # Linker object identity stays internal and never points at a legacy input.
        row['object'] = str(Path(row.pop('legacy')).with_suffix('.o'))
        if row['bundle'] not in final_bundles:
            owner = next((p for p in final_bundles if Path(row['source']).is_relative_to(Path(p).parent)),None)
            row['bundle'] = owner or 'assets/shared/asset.yaml'
    if any(r['bundle']=='assets/shared/asset.yaml' for r in rows.values()):
        shared = dict(schema_version=2,id='shared',models=[],animations=[],files=[])
        (root/'assets/shared/asset.yaml').write_bytes(dump(shared))
        final_bundles['assets/shared/asset.yaml'] = shared
    layout['schema_version'] = 2
    (root/LAYOUT).write_bytes(dump(layout))
    (root/'assets/layout/previews.yaml').write_bytes(dump(dict(schema_version=1,models=final_defs)))
    (root/'assets/index.yaml').write_bytes(dump(dict(schema_version=2,bundles=[dict(id=b['id'],manifest=p,
        models=[m['name'] for m in b.get('models',[])]) for p,b in sorted(final_bundles.items())])))


def validate_structure(root):
    for name in LEGACY_DIRS:
        if (root/'assets'/name).exists():
            raise ValueError(f'legacy extraction directory still present: assets/{name}')
    index = load_yaml(root/'assets/index.yaml')
    for entry in index['bundles']:
        path = root/entry['manifest']
        b = load_yaml(path)
        if b['schema_version']!=2:
            raise ValueError(f'old bundle schema: {path}')
    if len(list((root/'assets/items').glob('*/asset.yaml')))!=11:
        raise ValueError('not all eleven gameplay items have bundles')
    if len(list((root/'assets/courses').glob('*/asset.yaml')))!=10:
        raise ValueError('not all ten courses have bundles')
    for file in (root/'assets/courses').rglob('*.yaml'):
        if load_yaml(file).get('format','').startswith('course_'):
            raise ValueError(f'exposed course packing format: {file}')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path,default=ROOT)
    parser.add_argument('--check',action='store_true')
    args = parser.parse_args()
    root = args.root.resolve()
    if args.check:
        validate_structure(root)
        print('Semantic asset structure OK')
    else:
        upgrade(root,load_yaml(root/'tools/asset_registry.yaml'))


if __name__=='__main__':
    main()
