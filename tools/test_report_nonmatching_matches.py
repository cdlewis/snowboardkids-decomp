#!/usr/bin/env python3

import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from report_nonmatching_matches import full_unintegrated_scratch_results


class FullUnintegratedScratchResultsTests(unittest.TestCase):
    def test_returns_full_scratches_not_present_in_matchings(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            repo_root = Path(temp_dir)
            matchings = repo_root / "asm" / "matchings" / "test"
            matchings.mkdir(parents=True)
            (matchings / "integratedFunction.s").write_text("glabel integratedFunction\n")

            scratches_path = repo_root / "scratches.json"
            scratches_path.write_text(
                json.dumps(
                    [
                        {
                            "name": "unintegratedFunction",
                            "score": 0,
                            "max_score": 100,
                            "slug": "full1",
                        },
                        {
                            "name": "integratedFunction",
                            "score": 0,
                            "max_score": 100,
                            "slug": "full2",
                        },
                        {
                            "name": "partialFunction",
                            "score": 1,
                            "max_score": 100,
                            "slug": "part1",
                        },
                    ]
                )
            )

            results = full_unintegrated_scratch_results(repo_root, [scratches_path])

            self.assertEqual([result.function for result in results], ["unintegratedFunction"])
            self.assertEqual(results[0].slug, "full1")


if __name__ == "__main__":
    unittest.main()
