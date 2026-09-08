import tempfile
from pathlib import Path
import unittest
from unittest.mock import patch

from tools.asset_name_migration import migrate_names, registry, rewriters
from tools.asset_bundles import LAYOUT, dump, pack_segment
from tools.course_surface_data_common import load_yaml


class AssetNameTests(unittest.TestCase):
    def test_registry_matches_config(self):
        root = Path(__file__).resolve().parents[1]
        entries = registry(root)
        self.assertEqual(len(entries),28)
        config = load_yaml(root/'snowboardkids.yaml')
        segments = {s['start']:s for s in config['segments'] if isinstance(s,dict) and 'start' in s}
        for entry in entries:
            self.assertEqual(segments[entry['rom_start']]['name'],entry['name'])
            self.assertTrue(entry['evidence'])

    def test_rewrites_identifiers_not_raw_data_or_offsets(self):
        text,reference,document = rewriters([dict(old='_1E19C0',name='SLASH_MODEL_RESOURCES')])
        self.assertEqual(text('_1E19C0_ROM_START'),'SLASH_MODEL_RESOURCES_ROM_START')
        self.assertEqual(reference('assets/1e19c0/_1E19C0_vertices_000100.yaml'),
                         'assets/slash_model_resources/SLASH_MODEL_RESOURCES_vertices_000100.yaml')
        self.assertEqual(document(dict(data='1e19c0',offset=0x1e19c0)),dict(data='1e19c0',offset=0x1e19c0))
        self.assertEqual(text('0x1E19C0 unrelated_1E19C0'),'0x1E19C0 unrelated_1E19C0')

    def fixture(self,root):
        (root/LAYOUT).parent.mkdir(parents=True)
        (root/'assets/_123.c').write_text('Gfx _123[] = { {{0xB8000000, 0x12345678}}, };')
        row = dict(name='_123',rom_start=0x123,rom_end=0x12b,decoder='course_display_list',
                   source='assets/_123.c',bundle='assets/asset.yaml',object='assets/_123.o')
        (root/LAYOUT).write_bytes(dump(dict(schema_version=2,segments=[row])))
        (root/'assets/asset.yaml').write_bytes(dump(dict(id='test',models=[])))
        return row

    def test_migration_preserves_edits_and_is_idempotent(self):
        with tempfile.TemporaryDirectory() as d:
            root=Path(d)
            row=self.fixture(root)
            baseline=pack_segment(root,row)
            with patch('tools.asset_name_migration.registry',return_value=[dict(old='_123',name='MODEL',rom_start=0x123)]):
                migrate_names(root)
                migrated=load_yaml(root/LAYOUT)['segments'][0]
                self.assertEqual(migrated['source'],'assets/MODEL.c')
                self.assertEqual(migrated['object'],'assets/MODEL.o')
                self.assertEqual(pack_segment(root,migrated),baseline)
                migrate_names(root)
            self.assertEqual(len(list((root/'asset-backups').iterdir())),1)

    def test_destination_conflict_preserves_original(self):
        with tempfile.TemporaryDirectory() as d:
            root=Path(d)
            self.fixture(root)
            (root/'assets/MODEL.c').write_text('existing edit')
            with patch('tools.asset_name_migration.registry',return_value=[dict(old='_123',name='MODEL',rom_start=0x123)]):
                with self.assertRaisesRegex(ValueError,'conflicting'):
                    migrate_names(root)
            self.assertTrue((root/'assets/_123.c').exists())
            self.assertEqual((root/'assets/MODEL.c').read_text(),'existing edit')
