#!/usr/bin/env python3

import unittest
from pathlib import Path

import textconv


class TextConverterTests(unittest.TestCase):
    def setUp(self):
        self.characters, self.named_values = textconv.parse_charmap(Path(__file__).with_name("charmap.txt"))

    def test_encodes_glyphs_controls_and_terminator(self):
        values = textconv.encode_text(
            "{COLOR:2}A 9{END}{CREDITS_MANAGER_LEFT}Z{CREDITS_MANAGER_RIGHT}",
            self.characters,
            self.named_values,
        )
        self.assertEqual(values, [0xFFFC, 2, 0xA, 0xFFFE, 9, 0xFFFF, 0xF8, 0x23, 0xF9, 0xFFFF])

    def test_supports_newline_null_and_named_controls(self):
        values = textconv.encode_text(
            r"A\nB\0{PAGE_BREAK}{TITLE_LEFT}C{TITLE_RIGHT}",
            self.characters,
            self.named_values,
        )
        self.assertEqual(values, [0xA, 0xFFFD, 0xB, 0xFFFF, 0xFFFB, 0xF5, 0xC, 0xF6, 0xFFFF])

    def test_encodes_menu_punctuation(self):
        values = textconv.encode_text("IT'S: OK, YES!? -/\"", self.characters, self.named_values)
        self.assertEqual(
            values,
            [0x12, 0x1D, 0x27, 0x1C, 0x2E, 0xFFFE, 0x18, 0x14, 0x2A,
             0xFFFE, 0x22, 0xE, 0x1C, 0x24, 0x30, 0xFFFE, 0x2B, 0x2D,
             0x25, 0xFFFF],
        )

    def test_all_opted_in_source_text_is_encodable(self):
        source_root = Path(__file__).resolve().parents[1] / "src"
        for path in source_root.rglob("*.c"):
            source = path.read_text()
            if '"font_encoding.h"' not in source:
                continue
            with self.subTest(path=path.relative_to(source_root)):
                textconv.process_source(source, self.characters, self.named_values)
                self.assertNotRegex(source, r"\{0x[0-9A-Fa-f]+\}")

    def test_converts_only_opted_in_sources(self):
        plain = 'u16 text[] = { _("A") };\n'
        self.assertEqual(textconv.process_source(plain, self.characters, self.named_values), plain)

        opted_in = '#include "font_encoding.h"\nu16 text[] = { _("A") };\n'
        self.assertEqual(
            textconv.process_source(opted_in, self.characters, self.named_values),
            '#include "font_encoding.h"\nu16 text[] = { 0x000A, 0xFFFF };\n',
        )

    def test_converts_unterminated_fragment(self):
        source = '#include "font_encoding.h"\nu16 text[] = { _FRAGMENT("AB") };\n'
        self.assertEqual(
            textconv.process_source(source, self.characters, self.named_values),
            '#include "font_encoding.h"\nu16 text[] = { 0x000A, 0x000B };\n',
        )

    def test_ignores_macro_syntax_in_comments(self):
        source = '#include "font_encoding.h"\n// _("BAD")\nu16 text[] = { _("A") };\n'
        converted = textconv.process_source(source, self.characters, self.named_values)
        self.assertIn('// _("BAD")', converted)
        self.assertIn("{ 0x000A, 0xFFFF }", converted)

    def test_rejects_unknown_directive_character_and_color(self):
        for source in ("{NOPE}", "a", "{COLOR:65536}"):
            with self.subTest(source=source):
                with self.assertRaises(textconv.TextConversionError):
                    textconv.encode_text(source, self.characters, self.named_values)


if __name__ == "__main__":
    unittest.main()
