import unittest

from tools.splat_ext.course_display_list import N64SegCourse_display_list


class CourseDisplayListTests(unittest.TestCase):
    def test_raw_words_preserve_each_big_endian_command(self):
        data = bytes.fromhex("E700000000000000 B800000000000000")

        source = N64SegCourse_display_list._raw_words(data)

        self.assertIn("{{0xE7000000, 0x00000000}}", source)
        self.assertIn("{{0xB8000000, 0x00000000}}", source)
        self.assertEqual(source.count("Exact source word"), 2)


if __name__ == "__main__":
    unittest.main()
