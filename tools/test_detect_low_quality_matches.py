#!/usr/bin/env python3

import importlib.util
import tempfile
import unittest
from pathlib import Path


MODULE_PATH = Path(__file__).with_name("detect_low_quality_matches.py")
SPEC = importlib.util.spec_from_file_location("detect_low_quality_matches", MODULE_PATH)
detector = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(detector)


class DetectLowQualityMatchesTest(unittest.TestCase):
    def analyze(self, source):
        with tempfile.NamedTemporaryFile(mode="w", suffix=".c") as source_file:
            source_file.write(source)
            source_file.flush()
            return detector.analyze_file_for_violations(source_file.name)

    def test_skips_non_matching_function_attempt(self):
        stats = self.analyze(
            """
#pragma GLOBAL_ASM("asm/nonmatchings/example.s")
#ifdef NON_MATCHING
void unmatched(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x10) = 1;
}
#endif
void matched(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x14) = 2;
}
"""
        )

        self.assertNotIn("unmatched", stats)
        self.assertIn("matched", stats)

    def test_skips_if_defined_and_if_zero_attempts(self):
        stats = self.analyze(
            """
#if defined(NON_MATCHING)
void defined_attempt(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x10) = 1;
}
#endif
#if 0
void disabled_attempt(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x14) = 2;
}
#endif
"""
        )

        self.assertEqual({}, stats)

    def test_keeps_normal_branch(self):
        stats = self.analyze(
            """
#ifdef NON_MATCHING
void unmatched(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x10) = 1;
}
#else
void matched(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x14) = 2;
}
#endif
"""
        )

        self.assertNotIn("unmatched", stats)
        self.assertIn("matched", stats)


if __name__ == "__main__":
    unittest.main()
