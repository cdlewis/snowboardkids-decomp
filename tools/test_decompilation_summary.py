#!/usr/bin/env python3

from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from decompilation_summary import (
    RemainingFunction,
    best_percent,
    best_source,
    find_remaining_functions,
    render_document,
)
from report_nonmatching_matches import MatchResult, ScratchResult


class RemainingFunctionsTest(unittest.TestCase):
    def test_only_glabels_are_reported(self) -> None:
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            asm = root / "asm" / "nonmatchings" / "segment"
            asm.mkdir(parents=True)
            (asm / "function.s").write_text("nonmatching example, 0x24\n\nglabel example\n")
            (asm / "data.s").write_text(".section .rodata\n\ndlabel D_80000000\n")

            functions = find_remaining_functions(root)

            self.assertEqual([function.name for function in functions], ["example"])
            self.assertEqual(functions[0].size, 0x24)

    def test_best_score_can_come_from_scratch(self) -> None:
        local = MatchResult("example", 98.0, "base.c", Path("."))
        scratch = ScratchResult("example", 99.0, 1, 100, "abcde", False, "racer")

        self.assertEqual(best_percent(local, scratch), 99.0)
        self.assertEqual(best_source(local, scratch), "decomp.me · racer")

    def test_render_document_replaces_summary_markers(self) -> None:
        function = RemainingFunction("example", Path("asm/nonmatchings/example.s"), 16)
        local = MatchResult("example", 90.0, "base.c", Path("."))
        document = render_document(
            "{{REMAINING_COUNT}} {{SCORED_COUNT}} {{FUNCTION_ROWS}} {{CODE_PERCENT}}",
            [function],
            {"example": local},
            {},
            {"matched_code_percent": 93.5},
        )

        self.assertIn("1 1", document)
        self.assertIn("example", document)
        self.assertIn("93.50%", document)
        self.assertNotIn("{{", document)


if __name__ == "__main__":
    unittest.main()
