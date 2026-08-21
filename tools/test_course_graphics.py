import unittest

from tools.course_graphics_common import (
    collect_course_texture_references,
    pack_course_model_resources,
    scan_course_resource_commands,
    trace_course_graphics,
)


class CourseGraphicsTests(unittest.TestCase):
    def test_trace_follows_segment_two_calls_and_records_segment_three_vertices(self):
        nested = bytes.fromhex("0400081F03000010 B800000000000000")
        root = bytes.fromhex("0600000002000000 B800000000000000")
        bundle = nested + bytes(0x10) + root

        graph = trace_course_graphics(bundle, [0x20])

        self.assertEqual([(node.offset, node.end) for node in graph.display_lists], [(0, 0x10), (0x20, 0x30)])
        self.assertEqual(len(graph.vertex_references), 1)
        self.assertEqual(graph.vertex_references[0].segment, 3)
        self.assertEqual(graph.vertex_references[0].offset, 0x10)
        self.assertEqual(graph.vertex_references[0].count, 2)

    def test_model_resource_manifest_packs_raw_and_vertices(self):
        manifest = {
            "decompressed_size": 0x14,
            "parts": [
                {"type": "raw", "offset": 0, "data": "aabbccdd"},
                {
                    "type": "vertices",
                    "offset": 4,
                    "vertices": [
                        {"x": 1, "y": -2, "z": 3, "flag": 4, "s": -5, "t": 6, "r": 7, "g": 8, "b": 9, "a": 10}
                    ],
                },
            ],
        }

        packed = pack_course_model_resources(manifest)

        self.assertEqual(packed, bytes.fromhex("aabbccdd 0001fffe00030004fffb00060708090a"))

    def test_collects_ci4_texture_and_palette_loads(self):
        bundle = bytes.fromhex(
            "FD50000003000000 F550000007094260 E600000000000000 F3000000071FF200 "
            "E700000000000000 F540080000094260 F2000000007FC3FC FD10000003000400 "
            "E800000000000000 F500010007000000 E600000000000000 F00000000703C000 "
            "E700000000000000 B800000000000000"
        )
        graph = trace_course_graphics(bundle, [0])

        textures, palettes = collect_course_texture_references(bundle, graph, 0x500)

        self.assertEqual(
            [(item.offset, item.size, item.format, item.width, item.height) for item in textures],
            [(0, 0x400, "ci4", 64, 32)],
        )
        self.assertEqual([(item.offset, item.colors) for item in palettes], [(0x400, 16)])

    def test_model_resource_manifest_packs_texture_and_palette(self):
        manifest = {
            "decompressed_size": 8,
            "parts": [
                {"type": "texture", "offset": 0, "format": "ci4", "width": 4, "height": 2, "data": "01234567"},
                {"type": "palette", "offset": 4, "format": "rgba16", "colors": 2, "values": [0x89AB, 0xCDEF]},
            ],
        }

        self.assertEqual(pack_course_model_resources(manifest), bytes.fromhex("0123456789abcdef"))

    def test_scans_packed_lists_without_a_known_root(self):
        bundle = bytes.fromhex("0400081F03000010 B800000000000000")

        vertices, textures, palettes = scan_course_resource_commands(bundle, 0x30)

        self.assertEqual([(item.offset, item.count) for item in vertices], [(0x10, 2)])
        self.assertEqual(textures, [])
        self.assertEqual(palettes, [])


if __name__ == "__main__":
    unittest.main()
