"""Assemble static GLB components, preserving textures and placement transforms."""
import copy
import json
import math
import struct


def assemble(root, definition, definitions):
    from tools.model_preview import GLB
    out = GLB()
    fields = ('bufferViews', 'accessors', 'images', 'samplers', 'textures', 'materials', 'meshes', 'nodes')
    triangles = meshes = 0
    for component in definition['model']['components']:
        source = definitions[component['model']]
        path = root/source['bundle']
        blob = (path.parent/(source['model']['name']+'.glb')).read_bytes()
        length = struct.unpack_from('<I',blob,12)[0]
        doc = json.loads(blob[20:20+length])
        binary = blob[28+length:]
        offsets = {k:len(out.doc.setdefault(k,[])) for k in fields}
        while len(out.data)%4:
            out.data.append(0)
        binary_offset = len(out.data)
        out.data.extend(binary)
        for field in fields:
            values = copy.deepcopy(doc.get(field,[]))
            for value in values:
                if field=='bufferViews':
                    value['byteOffset'] = value.get('byteOffset',0)+binary_offset
                elif field in ('accessors','images') and 'bufferView' in value:
                    value['bufferView'] += offsets['bufferViews']
                elif field=='textures':
                    for key,target in (('source','images'),('sampler','samplers')):
                        if key in value: value[key] += offsets[target]
                elif field=='materials':
                    def textures(v):
                        if isinstance(v,dict):
                            for k,child in v.items():
                                if k.endswith('Texture') and isinstance(child,dict): child['index'] += offsets['textures']
                                else: textures(child)
                    textures(value)
                elif field=='meshes':
                    meshes += 1
                    for primitive in value['primitives']:
                        accessor = primitive.get('indices',primitive['attributes']['POSITION'])
                        triangles += doc['accessors'][accessor]['count']//3
                        primitive['attributes'] = {k:v+offsets['accessors'] for k,v in primitive['attributes'].items()}
                        if 'indices' in primitive: primitive['indices'] += offsets['accessors']
                        if 'material' in primitive: primitive['material'] += offsets['materials']
                elif field=='nodes':
                    if 'mesh' in value: value['mesh'] += offsets['meshes']
                    if 'children' in value: value['children'] = [n+offsets['nodes'] for n in value['children']]
            out.doc[field].extend(values)
        angle = component.get('rotation_y',0)*math.pi/4096
        node = out.node(component.get('name',component['model']),
                        children=[n+offsets['nodes'] for n in doc['scenes'][doc.get('scene',0)]['nodes']],
                        translation=component.get('position',[0,0,0]),rotation=[0,math.sin(angle),0,math.cos(angle)])
        out.doc['scenes'][0]['nodes'].append(node)
    dest = (root/definition['bundle']).parent/(definition['model']['name']+'.glb')
    out.write(dest)
    return dict(path=str(dest.relative_to(root)),meshes=meshes,triangles=triangles,animations=0)
