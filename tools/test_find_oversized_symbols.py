#!/usr/bin/env python3

import tempfile
import unittest
from pathlib import Path

from tools import find_oversized_symbols as finder


class OversizedSymbolsTests(unittest.TestCase):
    def setUp(self):
        self.temp_dir = tempfile.TemporaryDirectory()
        self.symbol_addrs = Path(self.temp_dir.name) / "symbol_addrs.txt"

    def tearDown(self):
        self.temp_dir.cleanup()

    def write_symbols(self, *lines):
        self.symbol_addrs.write_text("\n".join(lines) + "\n")

    def test_reports_object_larger_than_recorded_size(self):
        self.write_symbols(
            "gSmall = 0x80100000; // size:0x4",
            "gExact = 0x80100010; // rom:0x100 size:0x8",
        )
        output = """
  10: 80100000    16 OBJECT  GLOBAL DEFAULT    6 gSmall
  11: 80100010     8 OBJECT  GLOBAL DEFAULT    6 gExact
"""

        mismatches = finder.find_oversized_symbols(self.symbol_addrs, output)

        self.assertEqual([item.recorded.name for item in mismatches], ["gSmall"])
        self.assertEqual(mismatches[0].linked_size, 0x10)

    def test_uses_largest_matching_nonmatching_or_regular_symbol(self):
        self.write_symbols("gData = 0x800B9556; // size:0x1A")
        output = """
  10: 800b9556     1 OBJECT  GLOBAL DEFAULT    5 gData.NON_MATCHING
  11: 800b9556    38 OBJECT  GLOBAL DEFAULT    5 gData
  12: 800b9999   100 OBJECT  GLOBAL DEFAULT    5 gData
"""

        mismatches = finder.find_oversized_symbols(self.symbol_addrs, output)

        self.assertEqual(len(mismatches), 1)
        self.assertEqual(mismatches[0].linked_size, 0x26)

    def test_ignores_unsized_symbols_functions_and_undefined_objects(self):
        self.write_symbols(
            "gUnsized = 0x80100000;",
            "doThing = 0x80001000; // type:func size:0x4",
            "gDefined = 0x80100004; // size:4",
        )
        output = """
  10: 80100000    64 OBJECT  GLOBAL DEFAULT    6 gUnsized
  11: 80001000    16 OBJECT  GLOBAL DEFAULT    3 doThing
  12: 80100004    32 OBJECT  GLOBAL DEFAULT  UND gDefined
"""

        self.assertEqual(
            finder.find_oversized_symbols(self.symbol_addrs, output), []
        )

    def test_parses_hexadecimal_readelf_sizes(self):
        self.write_symbols("gLarge = 0x80100000; // size:0x100")
        output = "  10: 80100000 0x25800 OBJECT GLOBAL DEFAULT 6 gLarge\n"

        mismatches = finder.find_oversized_symbols(self.symbol_addrs, output)

        self.assertEqual(mismatches[0].linked_size, 0x25800)


if __name__ == "__main__":
    unittest.main()
