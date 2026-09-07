"""Deterministic glTF previews from canonical N64 sources (never build inputs)."""
from __future__ import annotations

import copy
import io
import json
import math
from pathlib import Path
import re
import struct

import png
from tools.asset_images import rgba16
from tools.asset_bundles import pack_segment, materialize, commands
from tools.course_surface_data_common import load_yaml


class GLB:
    def __init__(self):
        self.data = bytearray()
        self.doc = dict(asset=dict(version='2.0', generator='Snowboard Kids source preview'),
                        scene=0, scenes=[dict(nodes=[])], nodes=[], meshes=[], accessors=[],
                        bufferViews=[], buffers=[], materials=[], images=[], textures=[], samplers=[])

    def blob(self, data):
        self.data.extend(b'\0' * (-len(self.data) % 4))
        index = len(self.doc['bufferViews'])
        self.doc['bufferViews'].append(dict(buffer=0, byteOffset=len(self.data), byteLength=len(data)))
        self.data.extend(data)
        return index

    def accessor(self, values, width):
        flat = [x for value in values for x in value]
        index = len(self.doc['accessors'])
        self.doc['accessors'].append(dict(bufferView=self.blob(struct.pack(f'<{len(flat)}f', *flat)),
            componentType=5126, count=len(values), type={1:'SCALAR', 2:'VEC2', 3:'VEC3', 4:'VEC4'}[width],
            min=[min(v[i] for v in values) for i in range(width)], max=[max(v[i] for v in values) for i in range(width)]))
        return index

    def node(self, name, **kwargs):
        index = len(self.doc['nodes'])
        self.doc['nodes'].append(dict(name=name, **kwargs))
        return index

    def write(self, path):
        if not self.doc['meshes']:
            raise ValueError('display-list root contains no triangles')
        self.data.extend(b'\0' * (-len(self.data) % 4))
        self.doc['buffers'] = [dict(byteLength=len(self.data))]
        doc = {k: v for k, v in self.doc.items() if v != []}
        header = json.dumps(doc, separators=(',', ':'), allow_nan=False).encode()
        header += b' ' * (-len(header) % 4)
        output = struct.pack('<III', 0x46546C67, 2, 28+len(header)+len(self.data))
        output += struct.pack('<II', len(header), 0x4E4F534A)+header
        output += struct.pack('<II', len(self.data), 0x004E4942)+self.data
        path.write_bytes(output)


class Interpreter:
    """F3DEX execution state persists across calls; cache mutations are per draw."""
    def __init__(self, segments, textures, glb):
        self.segments, self.textures, self.glb = segments, textures, glb
        self.cache, self.tiles, self.loads, self.tlut = {}, {}, {}, {}
        self.image = None
        # system_runtime.c initializes the race renderer with lighting disabled;
        # these models predominantly contain baked vertex colors, not normals.
        self.geometry = 0
        self.enabled, self.tile = False, 0
        self.scale = (1., 1.)
        self.primitive = [1., 1., 1., 1.]
        self.combine = None
        self.materials, self.groups = {}, {}
        self.active = set()
        self.steps = 0

    def read(self, address, count):
        if not isinstance(address,int):
            raise ValueError('resource address has not been initialized')
        segment, offset = address >> 24, address & 0xFFFFFF
        data = self.segments.get(segment)
        if data is None or offset+count > len(data):
            raise ValueError(f'unmapped read 0x{address:08X}+0x{count:X}')
        return data[offset:offset+count]

    def texture(self):
        if not self.enabled:
            return None
        tile = self.tiles.get(self.tile)
        if tile is None:
            raise ValueError('textured draw without render tile')
        address = self.loads.get(tile['tmem'])
        if address not in self.textures:
            raise ValueError(f'no decoded texture for TMEM {tile["tmem"]}: {address}')
        texture = self.textures[address]
        colors = 16 if texture['format'] == 'ci4' else 256
        first = tile['palette']*16 if colors == 16 else 0
        if any(i not in self.tlut for i in range(first, first+colors)):
            raise ValueError(f'texture 0x{address:08X}: palette not loaded')
        palette = tuple(self.tlut[i] for i in range(first, first+colors))
        return address, texture, palette, tile

    def material(self):
        tex = self.texture()
        key = (None if tex is None else (tex[0], tex[2], tex[3]['cms'], tex[3]['cmt']),
               bool(self.geometry & 0x20000), tuple(self.primitive), tuple(self.combine or []))
        if key in self.materials:
            return self.materials[key], tex
        g = self.glb
        material = dict(name=f'material_{len(g.doc["materials"]):03d}', doubleSided=True,
                        pbrMetallicRoughness=dict(metallicFactor=0, roughnessFactor=1),
                        extras=dict(n64_combine=self.combine, n64_primitive_color=self.primitive,
                                    approximation='N64 lighting, fog and combiner effects are not emulated'))
        if not self.geometry & 0x20000:
            material['extensions'] = {'KHR_materials_unlit': {}}
            g.doc['extensionsUsed'] = ['KHR_materials_unlit']
        if tex:
            address, texture, palette, tile = tex
            width, height = texture['width'], texture['height']
            raw = self.read(address, (width*height*(4 if texture['format'] == 'ci4' else 8)+7)//8)
            indices = list(raw) if texture['format'] == 'ci8' else [v for b in raw for v in (b>>4,b&15)]
            rgba = [rgba16(v) for v in palette]
            pixels = [list(sum((rgba[i] for i in indices[y*width:(y+1)*width]), ())) for y in range(height)]
            stream = io.BytesIO()
            png.Writer(width, height, alpha=True, greyscale=False).write(stream, pixels)
            image = len(g.doc['images'])
            g.doc['images'].append(dict(bufferView=g.blob(stream.getvalue()), mimeType='image/png'))
            sampler = len(g.doc['samplers'])
            wrap = lambda mode: 33071 if mode & 2 else 33648 if mode & 1 else 10497
            g.doc['samplers'].append(dict(magFilter=9728, minFilter=9728, wrapS=wrap(tile['cms']), wrapT=wrap(tile['cmt'])))
            index = len(g.doc['textures'])
            g.doc['textures'].append(dict(source=image, sampler=sampler))
            material['pbrMetallicRoughness']['baseColorTexture'] = dict(index=index)
            if any(c[3] == 0 for c in rgba):
                material.update(alphaMode='MASK', alphaCutoff=0.5)
            material['extras'].update(texture_address=f'0x{address:08X}', palette_words=list(palette))
        index = len(g.doc['materials'])
        g.doc['materials'].append(material)
        self.materials[key] = index
        return index, tex

    def triangle(self, indices):
        if any(i not in self.cache for i in indices):
            raise ValueError(f'triangle references unloaded vertices {indices}')
        material, texture = self.material()
        group = self.groups.setdefault(material, [])
        for i in indices:
            vertex = copy.deepcopy(self.cache[i])
            if texture:
                _, t, _, tile = texture
                def uv(axis):
                    shift = tile['shifts' if axis == 0 else 'shiftt']
                    factor = 1/(1<<shift) if shift <= 10 else 1<<(16-shift)
                    origin = tile.get('uls' if axis == 0 else 'ult', 0)/4
                    return (vertex['uv'][axis]/32*self.scale[axis]*factor-origin) / t['width' if axis == 0 else 'height']
                vertex['uv'] = [uv(0), uv(1)]
            else:
                vertex['uv'] = [0., 0.]
            group.append(vertex)

    def run(self, address):
        if address in self.active:
            raise ValueError(f'recursive display list 0x{address:08X}')
        self.active.add(address)
        pc = address
        while True:
            self.steps += 1
            if self.steps > 1000000:
                raise ValueError('display-list execution budget exceeded')
            a, b = struct.unpack('>II', self.read(pc, 8))
            op = a >> 24
            try:
                if op == 0xB8:
                    break
                if op == 0x06:
                    self.run(b)
                    if (a >> 16) & 255:
                        break
                elif op == 0x04:
                    count, first = (a>>10)&63, ((a>>16)&255)//2
                    for i in range(count):
                        x,y,z,flag,s,t,r,g,blue,alpha = struct.unpack('>hhhHhhBBBB', self.read(b+i*16,16))
                        normal = [(v if v < 128 else v-256)/127 for v in (r,g,blue)]
                        length = math.sqrt(sum(v*v for v in normal))
                        normal = [v/length for v in normal] if length else [0.,1.,0.]
                        self.cache[first+i] = dict(position=[float(x),float(y),float(z)], uv=[s,t], normal=normal,
                            color=[1.,1.,1.,alpha/255] if self.geometry & 0x20000 else [v/255 for v in (r,g,blue,alpha)])
                elif op == 0xB2:
                    index, field = (a&65535)//2, (a>>16)&255
                    if index not in self.cache:
                        raise ValueError('modify unloaded vertex')
                    if field == 0x14:
                        self.cache[index]['uv'] = list(struct.unpack('>hh', struct.pack('>I',b)))
                    elif field == 0x10:
                        self.cache[index]['color'] = [v/255 for v in struct.pack('>I',b)]
                    else:
                        raise ValueError(f'unsupported vertex modification 0x{field:02X}')
                elif op in (0xBF, 0xB1):
                    if op == 0xB1:
                        self.triangle([(a>>shift&255)//2 for shift in (16,8,0)])
                    self.triangle([(b>>shift&255)//2 for shift in (16,8,0)])
                elif op == 0xB5:
                    v = [(b>>shift&255)//2 for shift in (24,16,8,0)]
                    self.triangle(v[:3]); self.triangle([v[0],v[2],v[3]])
                elif op == 0xFD:
                    self.image = b
                elif op == 0xF5:
                    self.tiles[(b>>24)&7] = dict(tmem=a&511, format=(a>>21)&7, size=(a>>19)&3,
                        line=(a>>9)&511, palette=(b>>20)&15, cmt=(b>>18)&3, cms=(b>>8)&3,
                        maskt=(b>>14)&15, masks=(b>>4)&15, shiftt=(b>>10)&15, shifts=b&15)
                elif op == 0xF2:
                    self.tiles[(b>>24)&7].update(uls=(a>>12)&4095, ult=a&4095)
                elif op == 0xF3:
                    self.loads[self.tiles[(b>>24)&7]['tmem']] = self.image
                elif op == 0xF0:
                    count = ((b>>14)&1023)+1
                    first = self.tiles[(b>>24)&7]['tmem']-256
                    palette = struct.unpack(f'>{count}H',self.read(self.image,count*2))
                    self.tlut.update({first+i:v for i,v in enumerate(palette)})
                elif op == 0xBB:
                    self.enabled, self.tile = bool(a&255), (a>>8)&7
                    self.scale = ((b>>16)/65536, (b&65535)/65536)
                elif op == 0xB6:
                    self.geometry &= ~b
                elif op == 0xB7:
                    self.geometry |= b
                elif op == 0xFA:
                    self.primitive = [v/255 for v in struct.pack('>I',b)]
                elif op == 0xFC:
                    self.combine = [a,b]
                elif op not in (0x00, 0x03, 0xB4, 0xB9, 0xBA, 0xBC, 0xBE, 0xE6, 0xE7, 0xE8, 0xE9, 0xF8, 0xF9, 0xFB):
                    raise ValueError(f'unsupported opcode 0x{op:02X}')
            except (ValueError, KeyError) as exc:
                raise ValueError(f'at 0x{pc:08X}: {exc}') from exc
            pc += 8
        self.active.remove(address)

    def mesh(self, name):
        if not self.groups:
            return None
        primitives = []
        for material, vertices in self.groups.items():
            attributes = {attr:self.glb.accessor([v[key] for v in vertices],width) for attr,key,width in
                          [('POSITION','position',3),('NORMAL','normal',3),('TEXCOORD_0','uv',2),('COLOR_0','color',4)]}
            primitives.append(dict(attributes=attributes, material=material, mode=4))
        index = len(self.glb.doc['meshes'])
        self.glb.doc['meshes'].append(dict(name=name,primitives=primitives))
        self.groups = {}
        return index


def quaternion(rotation):
    x,y,z = [v*math.pi/4096 for v in rotation]
    sx,cx,sy,cy,sz,cz = math.sin(x),math.cos(x),math.sin(y),math.cos(y),math.sin(z),math.cos(z)
    return [sx*cy*cz-cx*sy*sz, cx*sy*cz+sx*cy*sz, cx*cy*sz-sx*sy*cz, cx*cy*cz+sx*sy*sz]


def character_rig(root, index):
    text = (root/'src/race/motion/race_motion.c').read_text()
    def body(name):
        match = re.search(r'\b'+name+r'\[\]\s*=\s*\{(.*?)\};',text,re.S)
        if not match:
            raise ValueError(f'missing rig table {name}')
        return match[1]
    names = re.findall(r'gRaceMotionModelPartPositions\w+',body('gRaceMotionModelPartPositions'))
    positions = [list(map(int, re.findall(r'-?\d+', item))) for item in re.findall(r'\{([^{}]+)\}',body(names[index]))]
    counts = [int(v,0) for v in re.findall(r'0x[0-9A-Fa-f]+|\d+',body('gRaceMotionModelPartCounts'))]
    return positions, counts[8:22]


def posed_parts(positions, parents, frame):
    """The game composes translations through parents, but NOT rotations."""
    rotations = [quaternion(r) for r in frame['rotations']]
    rotations += [rotations[0]] * (len(positions)-len(rotations))
    offsets = copy.deepcopy(positions)
    offsets[:len(frame['offsets'])] = frame['offsets']
    translations = []
    for i, parent in enumerate(parents):
        if parent == 255:
            translations.append(list(offsets[i]))
            continue
        x,y,z,w = rotations[parent]
        vx,vy,vz = offsets[i]
        tx,ty,tz = 2*(y*vz-z*vy),2*(z*vx-x*vz),2*(x*vy-y*vx)
        rotated = [vx+w*tx+y*tz-z*ty,vy+w*ty+z*tx-x*tz,vz+w*tz+x*ty-y*tx]
        translations.append([a+b for a,b in zip(translations[parent],rotated)])
    return rotations, translations


def animate(glb, nodes, animations, positions, parents):
    glb.doc['animations'] = []
    for source_name, source in animations:
        for i, clip in enumerate(source['animations']):
            frames = clip['frames']
            if not frames:
                continue
            animation = dict(name=f'{source_name}_{i:03d}', samplers=[], channels=[],
                             extras=dict(timing='Reference samples at 60 Hz; runtime playback speed is caller-controlled'))
            times = glb.accessor([[f/60] for f in range(len(frames))],1)
            # Scene clips store absolute model-space translations as well as
            # absolute rotations; race clips instead use parent-relative offsets.
            poses = [([quaternion(r) for r in f['rotations']],f['offsets'])
                     if source['format']=='scene_animation' else posed_parts(positions,parents,f) for f in frames]
            for joint in range(len(nodes)):
                values = [pose[0][joint] for pose in poses]
                sampler = len(animation['samplers'])
                animation['samplers'].append(dict(input=times,output=glb.accessor(values,4),interpolation='STEP'))
                animation['channels'].append(dict(sampler=sampler,target=dict(node=nodes[joint],path='rotation')))
            for joint in range(len(nodes)):
                values = [pose[1][joint] for pose in poses]
                sampler = len(animation['samplers'])
                animation['samplers'].append(dict(input=times,output=glb.accessor(values,3),interpolation='STEP'))
                animation['channels'].append(dict(sampler=sampler,target=dict(node=nodes[joint],path='translation')))
            glb.doc['animations'].append(animation)
    if glb.doc['animations']:
        first = animations[0][1]['animations'][0]['frames'][0]
        rotations, translations = (([quaternion(r) for r in first['rotations']],first['offsets'])
            if animations[0][1]['format']=='scene_animation' else posed_parts(positions,parents,first))
        for i in range(len(nodes)):
            glb.doc['nodes'][nodes[i]].update(rotation=rotations[i],translation=translations[i])


def export_model(root, rows, bundle_path, bundle, model, cache):
    def payload(name):
        if name not in cache:
            cache[name] = pack_segment(root, rows[name], compressed=False)
        return cache[name]
    def resource(name):
        key = ('manifest',name)
        if key not in cache:
            cache[key] = materialize(root,rows[name])[0]
        return cache[key]
    glb = GLB()
    if 'source' in model:
        source = (root/model['source']).read_text()
        vertex_body = re.search(r'\b'+model['vertices']+r'\[.*?\]\s*=\s*\{(.*?)\};',source,re.S)[1]
        clean = re.sub(r'/\*.*?\*/|//[^\n]*','',vertex_body,flags=re.S)
        if re.sub(r'-?(?:0x[\dA-Fa-f]+|\d+)|[\s,{}]','',clean):
            raise ValueError('non-literal source vertex data')
        values = [int(v,0) for v in re.findall(r'-?(?:0x[\dA-Fa-f]+|\d+)',clean)]
        data = b''.join(struct.pack('>hhhHhhBBBB',*values[i:i+10]) for i in range(0,len(values),10))
        body = re.search(r'\b'+model['display_list']+r'\[.*?\]\s*=\s*\{(.*?)\};',source,re.S)[1]
        quads = [[int(v.strip(),0) for v in q.split(',')] for q in re.findall(r'gsSP1Quadrangle\(([^)]+)\)',body)]
        if not quads:
            raise ValueError('source model topology is not decoded')
        code = struct.pack('>II',0x04000000|((len(values)//10)<<10)|(len(data)-1),0x03000000)
        for a,b,c,d,flag in quads:
            if flag:
                raise ValueError('source quadrangle flag is unsupported')
            code += struct.pack('>II',0xB1000000|(a<<17)|(b<<9)|(c<<1),(a<<17)|(c<<9)|(d<<1))
        code += struct.pack('>II',0xB8000000,0)
        interpreter = Interpreter({2:code,3:data},{},glb)
        interpreter.run(0x02000000)
        glb.doc['scenes'][0]['nodes'] = [glb.node(model['name'],mesh=interpreter.mesh(model['name']))]
        path = bundle_path.parent/(model['name']+'.glb')
        glb.write(path)
        return dict(path=str(path.relative_to(root)),meshes=1,triangles=len(quads)*2,animations=0)
    if 'embedded' in model:
        name = model['embedded']
        segments = {2:payload(name)}
        manifest = resource(name)
        texture_segment = 2
    else:
        data = bytearray(model['graphics_end']-model['graphics_start'])
        written = bytearray(len(data))
        for name in model['graphics']:
            offset = rows[name]['rom_start']-model['graphics_start']
            blob = payload(name)
            if any(written[offset:offset+len(blob)]):
                raise ValueError('overlapping graphics address mapping')
            data[offset:offset+len(blob)] = blob
            written[offset:offset+len(blob)] = bytes([1])*len(blob)
        if not all(written):
            raise ValueError('incomplete graphics address mapping')
        segments = {2:bytes(data),3:payload(model['resource'])}
        manifest = resource(model['resource'])
        texture_segment = 3
    textures = {(texture_segment<<24)|p['offset']:p for p in manifest['parts'] if p['type']=='texture'}
    interpreter = Interpreter(segments,textures,glb)
    if 'setup' in model:
        interpreter.run(model['setup'])
    if 'texture_binding' in model:
        binding = model['texture_binding']
        sprites = resource(binding['asset'])
        entry = sprites['entries'][binding['entry']]
        image = next(p for p in sprites['images'] if p['offset']==entry['image_offset'])
        segments[4] = payload(binding['asset'])
        address = 0x04000000|image['offset']
        textures[address] = image
        interpreter.loads[0] = address
        mode = 0 if binding['wrap'] else 2
        interpreter.tiles[0] = dict(tmem=0,palette=0,cms=mode,cmt=mode,shifts=0,shiftt=0)
        interpreter.tlut = dict(enumerate(sprites['palettes'][entry['palette_index']]['values']))
    if 'vertex_binding' in model:
        binding = model['vertex_binding']
        segments[5] = struct.pack('>IIII',0x04000000|(binding['count']<<10)|(binding['count']*16-1),
                                  binding['address'],0xB8000000,0)
        interpreter.run(0x05000000)
    if 'character_index' in model:
        positions, parents = character_rig(root,model['character_index'])
        nodes = [glb.node(f'joint_{i:02d}',translation=position) for i,position in enumerate(positions)]
        for i,parent in enumerate(parents):
            glb.doc['scenes'][0]['nodes'].append(nodes[i])
            glb.doc['nodes'][nodes[i]]['extras'] = dict(n64_parent_part=parent)
        for i,offset in enumerate(model['roots']):
            interpreter.run(0x02000000|offset)
            mesh = interpreter.mesh(f'part_{i:02d}')
            if mesh is not None:
                glb.doc['nodes'][nodes[i+1]]['mesh'] = mesh
        animations = ([(model['scene_animation'],resource(model['scene_animation']))] if 'scene_animation' in model
                      else [(p,resource(p)) for p in model['animation_assets']] if 'animation_assets' in model
                      else [(Path(p).stem,load_yaml(bundle_path.parent/p)) for p in bundle['animations']])
        animate(glb,nodes,animations,positions,parents)
    else:
        for i,offset in enumerate(model['roots']):
            interpreter.run(0x02000000|offset)
            mesh = interpreter.mesh(model['name'])
            if mesh is not None:
                glb.doc['scenes'][0]['nodes'].append(glb.node(model['name'],mesh=mesh))
    glb.doc['extras'] = dict(source_bundle=str(bundle_path.relative_to(root)), source_model=model['name'],
                            axes='N64 model-local XYZ, Y up; 1 glTF unit = 1 stored vertex unit',
                            build_input=False)
    path = bundle_path.parent / (model['name']+'.glb')
    glb.write(path)
    return dict(path=str(path.relative_to(root)), meshes=len(glb.doc['meshes']),
                triangles=sum(p['attributes']['POSITION'] >= 0 and glb.doc['accessors'][p['attributes']['POSITION']]['count']//3
                              for m in glb.doc['meshes'] for p in m['primitives']),
                animations=len(glb.doc.get('animations',[])))


def export_all(root, layout, selection=None):
    rows = {r['name']:r for r in layout['segments']}
    report, cache = [], {}
    index = load_yaml(root/'assets/index.yaml')
    definitions = load_yaml(root/'assets/layout/previews.yaml')['models'] if layout.get('schema_version',1)>=2 else {}
    composites = []
    required = set()
    if selection:
        for key,definition in definitions.items():
            if 'components' in definition['model'] and selection in (key.rsplit('/',1)[0],definition['model']['name']):
                public = load_yaml(root/definition['bundle'])
                required.update(public[k] for k in ('terrain','backdrop') if k in public)
                required.update(public.get('effects',[]))
                required.update(c['model'] for c in public.get('scenery',[]))
    for bundle in sorted(set(b['manifest'] for b in index['bundles'])):
        path = root/bundle
        b = load_yaml(path)
        for model in b['models']:
            model = definitions.get(b['id']+'/'+model['name'],dict(model=model))['model']
            if 'components' in model:
                model['components'] = ([dict(model=b[k]) for k in ('terrain','backdrop') if k in b]
                                       + [dict(model=k) for k in b.get('effects',[])] + b.get('scenery',[]))
                if not selection or selection in (b['id'],model['name']):
                    composites.append((b,model))
                continue
            if selection and selection not in (b['id'],model['name']) and b['id']+'/'+model['name'] not in required:
                continue
            try:
                info = export_model(root, rows, path, b, model, cache)
                report.append(dict(bundle=b['id'],model=model['name'],status='exported',**info))
            except (ValueError,KeyError,OSError,struct.error) as exc:
                report.append(dict(bundle=b['id'],model=model['name'],status='error',error=str(exc)))
    from tools.model_scene import assemble
    for b,model in composites:
        try:
            info = assemble(root,definitions[b['id']+'/'+model['name']],definitions)
            report.append(dict(bundle=b['id'],model=model['name'],status='exported',**info))
        except (ValueError,KeyError,OSError,struct.error) as exc:
            report.append(dict(bundle=b['id'],model=model['name'],status='error',error=str(exc)))
    suffix = '.'+re.sub(r'[^a-zA-Z0-9_-]','_',selection) if selection else ''
    dest = root/f'build/model-preview-report{suffix}.json'
    dest.parent.mkdir(parents=True,exist_ok=True)
    dest.write_text(json.dumps(report,indent=2)+'\n')
    errors = [r for r in report if r['status']=='error']
    print(f'{len(report)-len(errors)} GLB exports; {len(errors)} errors; report: {dest}')
    if not report:
        raise ValueError(f'no models match {selection!r}')
    if errors:
        raise ValueError('\n'.join(f"{r['bundle']}/{r['model']}: {r['error']}" for r in errors))
