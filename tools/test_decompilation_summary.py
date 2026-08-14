#!/usr/bin/env python3

from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))

from decompilation_summary import (
    RemainingFunction,
    render_document,
    screenshot_preview,
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

    @patch("decompilation_summary.subprocess.run")
    @patch("decompilation_summary.find_chromium", return_value="/usr/bin/chromium")
    def test_screenshot_preview_uses_social_media_dimensions(
        self,
        _find_chromium,
        run,
    ) -> None:
        with tempfile.TemporaryDirectory() as directory:
            html_path = Path(directory) / "index.html"
            output_path = Path(directory) / "og.png"
            html_path.write_text("<html></html>")

            def write_screenshot(command, **_kwargs) -> None:
                screenshot_arg = next(arg for arg in command if arg.startswith("--screenshot="))
                Path(screenshot_arg.removeprefix("--screenshot=")).write_bytes(b"png")

            run.side_effect = write_screenshot
            screenshot_preview(html_path, output_path)

            command = run.call_args.args[0]
            self.assertIn("--window-size=1200,630", command)
            self.assertIn(f"--screenshot={output_path}", command)
            self.assertEqual(command[-1], html_path.as_uri())


if __name__ == "__main__":
    unittest.main()
