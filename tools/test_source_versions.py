import unittest

from tools.source_versions import region_source


class SourceVersionTests(unittest.TestCase):
    def test_region_and_elif(self):
        source = "#if defined(VERSION_JP)\njp\n#elif defined(VERSION_US)\nus\n#else\nneither\n#endif\n"
        for region in ("us", "jp"):
            selected = region_source(source, region)
            self.assertEqual(selected.strip(), region)
            self.assertEqual(selected.count("\n"), source.count("\n"))
            self.assertEqual(len(selected), len(source))

    def test_unknown_conditions_and_nested_regions(self):
        source = "#ifdef __clang__\n#if defined(VERSION_JP)\njp\n#else\nus\n#endif\n#else\ncompiler fallback\n#endif\n"
        selected = region_source(source)
        self.assertIn("#ifdef __clang__\n", selected)
        self.assertIn("#else\ncompiler fallback\n#endif\n", selected)
        self.assertIn("us\n", selected)
        self.assertNotIn("jp\n", selected)

    def test_ifndef_and_inactive_nested_else(self):
        source = "#ifndef VERSION_US\n#ifdef FEATURE\njp\n#else\nalso jp\n#endif\n#else\nus\n#endif\n"
        self.assertEqual(region_source(source).strip(), "us")
        self.assertIn("also jp", region_source(source, "jp"))

    def test_rejects_invalid_version(self):
        with self.assertRaises(ValueError):
            region_source("", "pal")


if __name__ == "__main__":
    unittest.main()
