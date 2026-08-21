import unittest

from tools.course_surface_data_common import pack_course_surface_data


class CourseSurfaceDataTests(unittest.TestCase):
    def test_semantic_manifest_fields_pack_to_native_layout(self):
        manifest = {
            "coordinates": [{"x": 1, "y": -2, "z": 3}],
            "faces": [
                {
                    "coordinate_indices": [0, 1, 2],
                    "surface_type": -3,
                    "skip_first_edge_check": 1,
                }
            ],
            "surfaces": [
                {
                    "neighbor_indices": [-1, 2, 3, 4],
                    "boundary_coordinate_indices": [5, 6, 7, 8],
                    "reference_coordinate_index": -9,
                    "path_angle": 0x1234,
                    "face_start_index": 9,
                    "face_end_index": 10,
                    "unknown_18": 0xABCD,
                    "edge_clamp_flags": 5,
                }
            ],
        }

        packed = pack_course_surface_data(manifest)

        self.assertEqual(
            packed,
            bytes.fromhex(
                "0001 0001fffe0003 "
                "0001 000000010002fd01 "
                "0001 "
                "ffff000200030004 0005000600070008 "
                "fff712340009000aabcd0005"
            ),
        )


if __name__ == "__main__":
    unittest.main()
