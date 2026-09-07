"""Run with blender --background --python this_file -- [--render-dir PATH]."""
import argparse
import json
import math
from pathlib import Path
import sys

import bpy
from mathutils import Vector


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report',type=Path,default=Path('build/model-preview-report.json'))
    parser.add_argument('--render-dir',type=Path)
    args = parser.parse_args(sys.argv[sys.argv.index('--')+1:] if '--' in sys.argv else [])
    report = json.loads(args.report.read_text())
    rendered = set()
    for entry in report:
        if entry['status'] != 'exported':
            raise ValueError(f'failed export: {entry}')
        bpy.ops.wm.read_factory_settings(use_empty=True)
        bpy.ops.import_scene.gltf(filepath=str(Path(entry['path']).resolve()))
        objects = [o for o in bpy.context.scene.objects if o.type=='MESH']
        if len(objects)!=entry['meshes'] or not objects:
            raise ValueError(f"mesh count mismatch: {entry['path']}")
        if entry['animations'] and not bpy.data.actions:
            raise ValueError(f"animations missing: {entry['path']}")
        category = ('pan' if entry['model']=='pan' else 'character' if entry['bundle']=='characters/slash'
                    else 'course' if entry['bundle']=='courses/rookie_mountain' else
                    'snowboard' if entry['model']=='board_000180_texture_00' else None)
        if args.render_dir and category and category not in rendered:
            rendered.add(category)
            render(objects,args.render_dir/category)
    print(f"BLENDER CHECK PASSED: {len(report)} self-contained GLBs imported")


def render(objects, path):
    scene = bpy.context.scene
    points = [o.matrix_world @ Vector(c) for o in objects for c in o.bound_box]
    lower = Vector([min(v[i] for v in points) for i in range(3)])
    upper = Vector([max(v[i] for v in points) for i in range(3)])
    center = (lower+upper)/2
    radius = max((upper-lower).length/2,1.)
    bpy.ops.object.camera_add(location=center+Vector((1.3,-1.8,1.1)).normalized()*radius*3)
    camera = bpy.context.object
    camera.rotation_euler = (center-camera.location).to_track_quat('-Z','Y').to_euler()
    camera.data.type = 'ORTHO'; camera.data.ortho_scale = radius*2.3
    camera.data.clip_end = radius*10
    scene.camera = camera
    scene.world = bpy.data.worlds.new('preview_world')
    scene.world.use_nodes = True
    scene.world.node_tree.nodes['Background'].inputs[0].default_value = (0.6,0.6,0.6,1)
    scene.world.node_tree.nodes['Background'].inputs[1].default_value = 0.8
    bpy.ops.object.light_add(type='SUN',location=center+Vector((radius,-radius,radius*2)))
    bpy.context.object.data.energy = 2
    bpy.context.object.rotation_euler = (math.radians(20),math.radians(-30),math.radians(-25))
    scene.render.engine = 'CYCLES'; scene.cycles.samples = 8
    scene.render.resolution_x = 512; scene.render.resolution_y = 512
    scene.render.resolution_percentage = 100
    path.parent.mkdir(parents=True,exist_ok=True)
    scene.render.filepath = str(path.with_suffix('.png').resolve())
    bpy.ops.render.render(write_still=True)


if __name__ == '__main__':
    main()
