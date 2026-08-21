import unittest

from tools.course_graphics_common import pack_course_model_resources, trace_course_graphics


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


if __name__ == "__main__":
    unittest.main()
