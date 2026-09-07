import tempfile
from pathlib import Path
import struct
import unittest

from tools.asset_images import read_indexed, write_indexed, externalize_sprites
from tools.animation_assets import parse_animation, pack_animation
from tools.audio_assets import decode_vadpcm, parse_pointer_bank
from tools.course_sprite_table_common import parse_course_sprite_table, pack_course_sprite_table
from tools.music_assets import parse_events, encode_music_part
from tools.readable_assets import parse_tilemap, pack_tilemap
from tools.textconv import parse_charmap, decode_text, encode_text
from tools.asset_parts import write_dependencies


class ReadableAssetTests(unittest.TestCase):
    def test_dependencies_refresh_when_image_path_changes(self):
        with tempfile.TemporaryDirectory() as directory:
            base = Path(directory)
            m = dict(images=[dict(path="first.png")])
            write_dependencies(base / "asset.yaml", m, base / "asset.bin")
            m["images"][0]["path"] = "second.png"
            write_dependencies(base / "asset.yaml", m, base / "asset.bin")
            deps = (base / "asset.d").read_text()
            self.assertIn("second.png", deps)
            self.assertNotIn("first.png", deps)

    def test_indexed_duplicate_colors_and_unused_nibble(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "pixels.png"
            write_indexed(path, bytes.fromhex("120f"), 3, 1, "ci4", [0xFFFF] * 16)
            self.assertEqual(read_indexed(path, 3, 1, "ci4", 15), bytes.fromhex("120f"))
            with self.assertRaisesRegex(ValueError, "expected indexed"):
                read_indexed(path, 2, 1, "ci4")

    def test_shared_sprite_image_uses_one_source(self):
        data = struct.pack(">II", 0, 2)
        data += struct.pack(">IHBB", 88, 0, 2, 1) + struct.pack(">IHBB", 88, 1, 2, 1)
        data += bytes(32) + bytes.fromhex("ffff") * 16 + b"\x12"
        m = dict(name="shared", decompressed_size=len(data), **parse_course_sprite_table(data))
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "shared.yaml"
            externalize_sprites(m, path)
            self.assertEqual(len(m["images"]), 1)
            self.assertEqual(pack_course_sprite_table(m, path.parent), data)
            # Changing one index affects both table entries, preserving shared storage.
            image = path.parent / m["images"][0]["path"]
            write_indexed(image, b"\x34", 2, 1, "ci4", [0] * 16)
            self.assertEqual(pack_course_sprite_table(m, path.parent), data[:-1] + b"\x34")

    def test_mixed_ci4_ci8_palette_slots(self):
        data = struct.pack(">II", 0, 2)
        data += struct.pack(">IHBB", 536, 0, 2, 1) + struct.pack(">IHBB", 537, 0, 2, 1)
        data += bytes(512) + b"\x12\x80\xff"
        m = dict(decompressed_size=len(data), **parse_course_sprite_table(data, [1], True))
        self.assertEqual([i["format"] for i in m["images"]], ["ci4", "ci8"])
        self.assertEqual(len(m["palettes"]), 16)
        self.assertEqual(pack_course_sprite_table(m), data)
        with self.assertRaises(ValueError):
            parse_course_sprite_table(data[:-1], [1], True)

    def test_packed_race_animation_reserved_bits(self):
        data = struct.pack(">Hh6h", 1, 1, -1, 2, -3, 4, -5, 6) + bytes.fromhex("12ab34ff") * 12
        m = dict(format="race_animation", **parse_animation(data, "race_animation"))
        self.assertEqual(m["animations"][0]["frames"][0]["rotations"][0], [0x128, 0xAB8, 0x348])
        self.assertEqual(pack_animation(m), data)
        m["animations"][0]["frames"][0]["rotations"][0][0] = 1
        with self.assertRaisesRegex(ValueError, "steps of 8"):
            pack_animation(m)

    def test_scene_animation(self):
        data = struct.pack(">Ih84h", 2, 1, *range(84))
        m = dict(format="scene_animation", **parse_animation(data, "scene_animation", 14))
        self.assertEqual(pack_animation(m), data)
        with self.assertRaises(ValueError):
            parse_animation(data[:-2], "scene_animation", 14)

    def test_tilemap_one_based_image_index_and_empty_tile(self):
        data = struct.pack(">8H", 2, 1, 2, 2, 2, 24, 28, 60)
        data += struct.pack(">hBBhBB2h", 0, 0, 0, 1, 0, 3, 0, 1) + bytes(32) + b"\x12\x34"
        m = dict(image_format="ci4", **parse_tilemap(data, "ci4"))
        self.assertEqual(pack_tilemap(m, Path(".")), data)
        m["tiles"][1]["image_index"] = 2
        with self.assertRaises(ValueError):
            pack_tilemap(m, Path("."))

    def test_adpcm_predictor_sign_and_history(self):
        book = dict(order=2, predictors=1, coefficients=[0] * 16)
        self.assertEqual(decode_vadpcm(bytes.fromhex("001f80") + bytes(6), book)[:4], [1, -1, -8, 0])
        # Predictor copies the last history sample across the next half-frame.
        book["coefficients"][8:] = [2048] * 8
        data = b"\0" + bytes.fromhex("0000000100000000")
        self.assertEqual(decode_vadpcm(data, book), [0]*7 + [1]*9)
        with self.assertRaisesRegex(ValueError, "predictor"):
            decode_vadpcm(b"\x01" + bytes(8), book)
        with self.assertRaises(ValueError):
            parse_pointer_bank(bytes(24))

    def test_music_loop_end_preserves_alignment_as_padding(self):
        data = bytes([0x95, 255, 0x99, 60, 127, 12, 0x96, 0, 0])
        events, stop = parse_events(data, 0, len(data))
        self.assertEqual(stop, 7)
        self.assertEqual(events[2]["note"], 60)
        self.assertEqual(encode_music_part(dict(type="sequence", offset=0, events=events)), data[:7])
        with self.assertRaises(ValueError):
            parse_events(b"\x81\x80", 0, 2)

    def test_text_unknown_words_controls_and_embedded_terminators(self):
        chars, names = parse_charmap(Path(__file__).with_name("charmap.txt"))
        words = [10, 0xFFFC, 2, 0xFFFD, 0xFFFF, 0xABCD, 0xFFFC]
        self.assertEqual(encode_text(decode_text(words, chars, names), chars, names, False), words)


if __name__ == "__main__":
    unittest.main()
