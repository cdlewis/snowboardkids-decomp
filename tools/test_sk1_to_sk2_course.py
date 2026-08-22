import struct
import unittest

from tools.sk1_to_sk2_course import (
    build_texture_table,
    compress_sno,
    convert_f3dex_command,
    decompress_sno,
    fixed17_to_fixed16,
)


class Sk1ToSk2CourseTests(unittest.TestCase):
    def test_sno_round_trip_and_alignment(self):
        source = b"snowboard kids" * 31 + bytes(range(64))
        compressed = compress_sno(source)
        self.assertEqual(len(compressed) % 0x10, 0)
        self.assertEqual(decompress_sno(compressed, len(source)), source)

    def test_fixed_point_conversion_rounds_negative_values_down(self):
        self.assertEqual(fixed17_to_fixed16(5), 2)
        self.assertEqual(fixed17_to_fixed16(-5), -3)

    def test_display_list_addresses_and_vertex_encoding_are_remapped(self):
        self.assertEqual(
            convert_f3dex_command(0x0400081F, 0x03001234), (0x01002004, 0x02001234)
        )
        self.assertEqual(
            convert_f3dex_command(0x06000000, 0x02005678), (0xDE000000, 0x01005678)
        )
        self.assertEqual(
            convert_f3dex_command(0xFD500000, 0x03009ABC), (0xFD500000, 0x02009ABC)
        )
        self.assertEqual(
            convert_f3dex_command(0xFD500000, 0x02009ABC), (0xFD500000, 0x01009ABC)
        )

    def test_triangles_and_end_are_f3dex2_commands(self):
        self.assertEqual(convert_f3dex_command(0xBF000000, 0x00020406), (0x05020406, 0))
        self.assertEqual(
            convert_f3dex_command(0xB1000204, 0x0006080A), (0x06000204, 0x0006080A)
        )
        self.assertEqual(convert_f3dex_command(0xB8000000, 123), (0xDF000000, 0))

    def test_texture_table_expands_sk1_entries_to_sk2_stride(self):
        source = bytearray(struct.pack(">II", 0, 1))
        source.extend(struct.pack(">IHBB", 0x30, 0, 4, 2))
        source.extend(bytes(range(0x20)))
        source.extend(bytes.fromhex("12345678"))
        packed, count = build_texture_table(bytes(source))
        self.assertEqual(count, 1)
        self.assertEqual(struct.unpack_from(">II", packed, 0), (0, 1))
        self.assertEqual(struct.unpack_from(">IHHH", packed, 8), (0x38, 0, 4, 2))
        self.assertEqual(packed[0x18:0x38], bytes(range(0x20)))
        self.assertEqual(packed[0x38:], bytes.fromhex("12345678"))


if __name__ == "__main__":
    unittest.main()
