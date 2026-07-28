#!/usr/bin/env python3

import tempfile
import unittest
from pathlib import Path

from tools import find_inconsistent_global_types as finder


class InconsistentGlobalTypesTests(unittest.TestCase):
    def setUp(self):
        self.temp_dir = tempfile.TemporaryDirectory()
        self.root = Path(self.temp_dir.name)
        (self.root / "src").mkdir()
        (self.root / "include").mkdir()

    def tearDown(self):
        self.temp_dir.cleanup()

    def write(self, relative_path, text):
        path = self.root / relative_path
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)
        return path

    def set_symbols(self, *lines):
        self.write("symbol_addrs.txt", "\n".join(lines) + "\n")

    def test_reports_only_known_data_symbols_with_different_types(self):
        self.set_symbols(
            "gValue = 0x80100000;",
            "gSame = 0x80100004;",
            "doThing = 0x80001000; // type:func",
        )
        self.write(
            "include/globals.h",
            "extern s16 gValue;\nextern u32 gSame;\nvoid doThing(void);\n",
        )
        self.write(
            "src/main.c",
            "u16 gValue;\nu32 gSame;\ns32 unknownSymbol;\n"
            "s32 doThing(void) { return 0; }\n",
        )

        self.assertEqual(finder.inconsistent_symbols(self.root), ["gValue"])

    def test_normalizes_storage_whitespace_and_array_bounds(self):
        self.set_symbols("gData = 0x80100000;")
        self.write("include/data.h", "extern const u8 gData[];\n")
        self.write(
            "src/data.c",
            "const u8\n"
            "gData[DATA_COUNT] = {\n"
            "    1, 2, 3,\n"
            "};\n",
        )

        self.assertEqual(finder.inconsistent_symbols(self.root), [])

    def test_detects_pointer_and_qualifier_differences(self):
        self.set_symbols(
            "gPointer = 0x80100000;",
            "gQualified = 0x80100004;",
        )
        self.write(
            "include/data.h",
            "extern Foo *gPointer;\nextern const s32 gQualified;\n",
        )
        self.write(
            "src/data.c",
            "extern Bar *gPointer;\ns32 gQualified;\n",
        )

        self.assertEqual(
            finder.inconsistent_symbols(self.root),
            ["gPointer", "gQualified"],
        )

    def test_ignores_comments_locals_and_function_declarations(self):
        self.set_symbols(
            "gValue = 0x80100000;",
            "namedLikeData = 0x80100004;",
        )
        self.write("include/data.h", "extern s32 gValue;\n")
        self.write(
            "src/main.c",
            "/* extern u8 gValue; */\n"
            "void useValue(void) {\n"
            "    u16 gValue;\n"
            "}\n"
            "void namedLikeData(void);\n",
        )

        self.assertEqual(finder.inconsistent_symbols(self.root), [])

    def test_ignores_generated_scratch_files(self):
        self.set_symbols("gValue = 0x80100000;")
        self.write("include/data.h", "extern s32 gValue;\n")
        self.write("src/" + "te" + "mp.c", "extern u8 gValue;\n")
        self.write("src/" + "c" + "tx.c", "extern u16 gValue;\n")

        self.assertEqual(finder.inconsistent_symbols(self.root), [])

    def test_compares_function_pointer_variables(self):
        self.set_symbols("gCallback = 0x80100000;")
        self.write("include/data.h", "extern void (*gCallback)(s32);\n")
        self.write("src/data.c", "void (*gCallback)(u32);\n")

        self.assertEqual(finder.inconsistent_symbols(self.root), ["gCallback"])


if __name__ == "__main__":
    unittest.main()
