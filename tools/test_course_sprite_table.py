import unittest

from tools.course_sprite_table_common import parse_course_sprite_table, pack_course_sprite_table


class CourseSpriteTableTests(unittest.TestCase):
    def test_parse_and_pack_sprite_table(self):
        data = bytes.fromhex(
            "0000000000000001 0000003000000402 "
            "00010003000500070009000b000d000f 00110013001500170019001b001d001f "
            "01234567"
        )

        parsed = parse_course_sprite_table(data)
        manifest = {"decompressed_size": len(data), **parsed}

        self.assertEqual(parsed["entry_count"], 1)
        self.assertEqual(parsed["entries"][0]["width"], 4)
        self.assertEqual(parsed["entries"][0]["height"], 2)
        self.assertEqual(len(parsed["palettes"]), 1)
        self.assertEqual(pack_course_sprite_table(manifest), data)

    def test_rejects_inconsistent_shared_image_dimensions(self):
        data = bytes.fromhex(
            "0000000000000002 0000003800000402 0000003800000204 "
            + "0000" * 16
            + "01234567"
        )

        with self.assertRaisesRegex(ValueError, "inconsistent dimensions"):
            parse_course_sprite_table(data)


if __name__ == "__main__":
    unittest.main()
