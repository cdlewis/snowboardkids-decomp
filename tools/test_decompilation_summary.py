#!/usr/bin/env python3

from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from decompilation_summary import (
    RemainingFunction,
    render_document,
)
from report_nonmatching_matches import ScratchResult


class RemainingFunctionsTest(unittest.TestCase):
    def test_render_document_replaces_summary_markers(self) -> None:
        function = RemainingFunction("example", Path("example"), 16)
        scratch = ScratchResult("example", 99.0, 1, 100, "abcde", False, "racer")
        document = render_document(
            "{{REMAINING_COUNT}} {{SCRATCH_COUNT}} {{FUNCTION_ROWS}} {{CODE_PERCENT}}",
            [function],
            {"example": scratch},
            {"matched_code_percent": 93.5},
        )

        self.assertIn("1 1", document)
        self.assertIn("example", document)
        self.assertIn("93.50%", document)
        self.assertNotIn("{{", document)

    def test_functions_without_published_matches_are_flagged(self) -> None:
        scratched = RemainingFunction("scratched", Path("scratched"), 16)
        local_only = RemainingFunction("localOnly", Path("localOnly"), 16)
        scratch = ScratchResult("scratched", 99.0, 1, 100, "abcde", False, "racer")

        document = render_document(
            "{{FUNCTION_ROWS}}",
            [scratched, local_only],
            {"scratched": scratch},
            {},
        )

        self.assertIn("scratched", document)
        self.assertIn("localOnly", document)
        self.assertIn("Needs match", document)


if __name__ == "__main__":
    unittest.main()
