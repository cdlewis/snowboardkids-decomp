import copy
import json
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

from tools.asset_bundles import (LAYOUT, commands, dependencies, dump, materialize,
                                migrate, pack_segment, safe_path)
from tools.model_preview import GLB, Interpreter, posed_parts, quaternion


def words(*pairs):
    return b''.join(struct.pack('>II', *pair) for pair in pairs)


def vertex(x, y, z, s=0, t=0):
    return struct.pack('>hhhHhhBBBB', x,y,z,0,s,t,255,255,255,255)


class BundleTests(unittest.TestCase):
    def test_format_specific_segment_adapters(self):
        import importlib
        from tools.asset_bundles import ASSET_TYPES, source_segments
        from tools.splat_ext.asset_segment import AssetSegment
        root = Path(__file__).resolve().parents[1]
        _,segments = source_segments(root)
        self.assertTrue(set(ASSET_TYPES).issubset({s['type'] for s in segments}))
        for segment in segments:
            self.assertNotEqual(segment['type'],'readable_asset')
            self.assertNotIn('asset_format',segment)
        for kind in ASSET_TYPES:
            module = importlib.import_module('tools.splat_ext.'+kind)
            cls = getattr(module,'N64Seg'+kind.capitalize())
            self.assertTrue(issubclass(cls,AssetSegment))
            instance = object.__new__(cls)
            instance.type = kind
            self.assertEqual(instance.statistics_type,kind)

    def test_extract_uses_segment_type(self):
        from tools.readable_assets import extract_asset, pack_asset
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)/'replay.yaml'
            data = bytes(4*2400*3)
            manifest = extract_asset(data,dict(type='replay',name='test',compression='none'),path)
            self.assertEqual(manifest['format'],'replay')
            self.assertEqual(pack_asset(manifest,path.parent),data)

    def test_existing_decoder_migration_preserves_sources_and_object_identity(self):
        from tools.semantic_assets import upgrade
        from tools.course_surface_data_common import load_yaml
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root/LAYOUT).parent.mkdir(parents=True)
            source = root/'replay.yaml'
            original = dump(dict(format='replay',schema_version=2,compression='none'))
            source.write_bytes(original)
            row = dict(name='test',decoder='readable_asset',source='replay.yaml',
                       object='assets/readable/test.o')
            (root/LAYOUT).write_bytes(dump(dict(schema_version=2,segments=[row])))
            with patch('tools.semantic_assets.validate_structure'):
                upgrade(root,{})
                upgrade(root,{})
            migrated = load_yaml(root/LAYOUT)['segments'][0]
            self.assertEqual(migrated,dict(row,decoder='replay'))
            self.assertEqual(source.read_bytes(),original)
            self.assertIn('$(BUILD_DIR)/assets/readable/test.o:',(root/'assets/layout/bundles.mk').read_text())

    def test_character_bundle_names(self):
        from tools.asset_bundles import owner, CHAR_RESOURCES, CHAR_ANIMATIONS
        names = ('slash','wendy','jam','linda','tommy','ninja')
        for bank in (CHAR_RESOURCES,CHAR_ANIMATIONS):
            for index,resource in enumerate(bank):
                self.assertEqual(owner(dict(name=resource)),f'assets/characters/{names[index]}')

    def test_semantic_registry_matches_runtime(self):
        from tools.semantic_assets import validate_registry, signed32
        from tools.course_surface_data_common import load_yaml
        root = Path(__file__).resolve().parents[1]
        validate_registry(root,load_yaml(root/'tools/asset_registry.yaml'))
        self.assertEqual(signed32(-65536),-65536)
        self.assertEqual(signed32(0xffff0000),-65536)

    def test_semantic_sections_are_build_inputs(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            (root/'mesh.yaml').write_bytes(dump(dict(coordinates=[1,2,3])))
            (root/'record.yaml').write_bytes(dump(dict(format='course_surface_data',sections=[dict(source='mesh.yaml')])))
            row = dict(source='record.yaml',compression='none',decoder='course_surface_data')
            self.assertEqual(materialize(root,row)[0]['coordinates'],[1,2,3])
            self.assertIn('mesh.yaml',dependencies(root,row))

    def test_commands_reject_nonliteral_edits(self):
        with tempfile.TemporaryDirectory() as d:
            path = Path(d)/'model.c'
            path.write_text('Gfx x[] = { {{0xB8000000, 0x00000000}}, };')
            self.assertEqual(commands(path), words((0xB8000000,0)))
            path.write_text('Gfx x[] = { {{0xB8000000, arbitrary}}, };')
            with self.assertRaises(ValueError):
                commands(path)

    def test_path_escape_rejected(self):
        with tempfile.TemporaryDirectory() as d:
            with self.assertRaises(ValueError):
                safe_path(Path(d),'../outside')

    def test_migration_preserves_edited_sources_and_refuses_conflicts(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            old = root/'assets/course_display_lists/MODEL.c'
            old.parent.mkdir(parents=True)
            original = 'Gfx MODEL[] = { {{0xB8000000, 0x12345678}}, };'
            old.write_text(original)
            (root/'snowboardkids.yaml').write_bytes(dump(dict(sha1='test', segments=[
                dict(start=4096,type='course_display_list',name='MODEL'),[4104]])))
            with patch('tools.asset_bundles.index_models'):
                migrate(root)
            layout = __import__('yaml').safe_load((root/LAYOUT).read_text())
            row = layout['segments'][0]
            self.assertEqual(pack_segment(root,row),words((0xB8000000,0x12345678)))
            self.assertEqual(old.read_text(),original)
            new = root/row['source']
            new.write_text(original.replace('12345678','87654321'))
            migrate(root,if_absent=True)
            self.assertEqual(pack_segment(root,row),words((0xB8000000,0x87654321)))
            with self.assertRaises(ValueError):
                migrate(root)

    def test_size_budget(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            (root/'model.c').write_text('Gfx x[] = { {{0xB8000000, 0x00000000}}, };')
            row = dict(name='x',decoder='course_display_list',source='model.c',rom_start=0,rom_end=16)
            with self.assertRaisesRegex(ValueError,'fixed ROM allocation'):
                pack_segment(root,row)

    def test_external_vertex_records_and_dependencies(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            (root/'vertices.yaml').write_bytes(dump(dict(vertices=[dict(x=1,y=2,z=3,flag=0,s=0,t=0,r=255,g=255,b=255,a=255)])))
            (root/'resource.yaml').write_bytes(dump(dict(format='course_model_resources',decompressed_size=16,
                parts=[dict(type='vertices',offset=0,source='vertices.yaml')])) )
            row = dict(source='resource.yaml',compression='none',decoder='model_resources')
            m,_ = materialize(root,row)
            self.assertEqual(m['parts'][0]['vertices'][0]['x'],1)
            self.assertIn('vertices.yaml',dependencies(root,row))
            self.assertEqual(pack_segment(root,row,compressed=False),vertex(1,2,3))


class PreviewTests(unittest.TestCase):
    def test_scene_assembly_rebases_meshes_and_placements(self):
        from tools.model_scene import assemble
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            glb = GLB()
            positions = glb.accessor([[0,0,0],[1,0,0],[0,1,0]],3)
            glb.doc['meshes'].append(dict(primitives=[dict(attributes=dict(POSITION=positions))]))
            glb.doc['scenes'][0]['nodes'] = [glb.node('triangle',mesh=0)]
            glb.write(root/'triangle.glb')
            definition = dict(bundle='asset.yaml',model=dict(name='preview',components=[
                dict(model='triangle'),dict(model='triangle',position=[10,20,30],rotation_y=1024)]))
            report = assemble(root,definition,dict(triangle=dict(bundle='asset.yaml',model=dict(name='triangle'))))
            blob = (root/'preview.glb').read_bytes()
            size = struct.unpack_from('<I',blob,12)[0]
            doc = json.loads(blob[20:20+size])
            self.assertEqual(report['meshes'],2)
            self.assertEqual(doc['nodes'][2]['mesh'],1)
            self.assertEqual(doc['nodes'][3]['translation'],[10,20,30])
            self.assertAlmostEqual(doc['nodes'][3]['rotation'][1],2**-.5)

    def test_runtime_default_uses_baked_vertex_colors(self):
        self.assertEqual(Interpreter({}, {}, GLB()).geometry & 0x20000,0)

    def test_empty_glb_is_not_reported_as_a_model(self):
        with tempfile.TemporaryDirectory() as d:
            with self.assertRaisesRegex(ValueError,'no triangles'):
                GLB().write(Path(d)/'empty.glb')

    def interpreter(self, code):
        vertices = vertex(0,0,0)+vertex(10,0,0)+vertex(0,10,0)
        i = Interpreter({2:code,3:vertices},{},GLB())
        i.geometry = 0
        return i

    def test_triangle_and_vertex_cache(self):
        i = self.interpreter(words((0x04000C2F,0x03000000),(0xBF000000,0x00000204),(0xB8000000,0)))
        i.run(0x02000000)
        self.assertEqual(len(next(iter(i.groups.values()))),3)

    def test_nested_branch_state(self):
        code = words((0x04000C2F,0x03000000),(0x06010000,0x02000020),
                     (0xFF000000,0),(0xFF000000,0),(0xBF000000,0x00000204),(0xB8000000,0))
        i = self.interpreter(code)
        i.run(0x02000000)
        self.assertEqual(len(next(iter(i.groups.values()))),3)

    def test_modify_vertex_preserves_previous_triangle(self):
        i = self.interpreter(words((0x04000C2F,0x03000000),(0xBF000000,0x00000204),
            (0xB2100000,0xFF0000FF),(0xBF000000,0x00000204),(0xB8000000,0)))
        i.run(0x02000000)
        vertices = next(iter(i.groups.values()))
        self.assertEqual(vertices[0]['color'],[1.,1.,1.,1.])
        self.assertEqual(vertices[3]['color'],[1.,0.,0.,1.])

    def test_unsupported_command_and_recursive_list_fail(self):
        for code in (words((0x01020040,0)),words((0x06000000,0x02000000))):
            with self.assertRaises(ValueError):
                self.interpreter(code).run(0x02000000)

    def test_uninitialized_texture_not_guessed(self):
        i = self.interpreter(words((0x04000C2F,0x03000000),(0xBB000001,0xFFFFFFFF),
                                  (0xBF000000,0x00000204),(0xB8000000,0)))
        with self.assertRaisesRegex(ValueError,'without render tile'):
            i.run(0x02000000)

    def test_tlut_slot_and_palette_switch(self):
        i = self.interpreter(words((0xFD100000,0x03000000),(0xF5000110,0x07000000),
                                  (0xF0000000,0x0703C000),(0xB8000000,0)))
        i.segments[3] = struct.pack('>16H',*range(16))
        i.run(0x02000000)
        self.assertEqual(i.tlut,{n+16:n for n in range(16)})

    def test_rotations_are_absolute_positions_follow_parents(self):
        positions = [[0,0,0],[10,0,0],[10,0,0]]
        frame = dict(offsets=[],rotations=[[0,0,1024],[0,0,0],[0,0,0]])
        rotations, translations = posed_parts(positions,[255,0,1],frame)
        self.assertAlmostEqual(translations[1][0],0,places=6)
        self.assertAlmostEqual(translations[1][1],10,places=6)
        self.assertAlmostEqual(translations[2][0],10,places=6)
        self.assertAlmostEqual(translations[2][1],10,places=6)
        self.assertEqual(rotations[1],quaternion([0,0,0]))

    def test_glb_is_deterministic_and_self_contained(self):
        i = self.interpreter(words((0x04000C2F,0x03000000),(0xBF000000,0x00000204),(0xB8000000,0)))
        i.run(0x02000000)
        node = i.glb.node('triangle',mesh=i.mesh('triangle'))
        i.glb.doc['scenes'][0]['nodes'].append(node)
        with tempfile.TemporaryDirectory() as d:
            a,b = Path(d)/'a.glb',Path(d)/'b.glb'
            i.glb.write(a); i.glb.write(b)
            self.assertEqual(a.read_bytes(),b.read_bytes())
            raw = a.read_bytes()
            magic,version,size = struct.unpack_from('<III',raw)
            self.assertEqual((magic,version,size),(0x46546C67,2,len(raw)))
            length = struct.unpack_from('<I',raw,12)[0]
            doc = json.loads(raw[20:20+length])
            self.assertNotIn('uri',doc['buffers'][0])


if __name__ == '__main__':
    unittest.main()
