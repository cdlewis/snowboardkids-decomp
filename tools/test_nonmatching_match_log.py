#!/usr/bin/env python3

from __future__ import annotations

import hashlib
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from nonmatching_match_log import certified_attempt_path, parse_match_log_line


TOOLS_DIR = Path(__file__).resolve().parent
DIST = TOOLS_DIR / "claude-decomp-env" / "dist.py"


class MatchLogTest(unittest.TestCase):
    def test_partial_legacy_entry_remains_valid(self) -> None:
        with tempfile.TemporaryDirectory() as temp:
            workspace = Path(temp)
            attempt = workspace / "base_1.c"
            attempt.write_text("partial\n")
            entry = parse_match_log_line("base_1.c 99.944%")

            self.assertIsNotNone(entry)
            assert entry is not None
            self.assertEqual(certified_attempt_path(entry, workspace), attempt)

    def test_legacy_full_match_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temp:
            workspace = Path(temp)
            (workspace / "base_1.c").write_text("uncertified\n")
            entry = parse_match_log_line("base_1.c 100.000%")

            self.assertIsNotNone(entry)
            assert entry is not None
            self.assertIsNone(certified_attempt_path(entry, workspace))

    def test_certified_archive_is_accepted(self) -> None:
        with tempfile.TemporaryDirectory() as temp:
            workspace = Path(temp)
            content = b"matching source\n"
            digest = hashlib.sha256(content).hexdigest()
            archive_dir = workspace / ".matches"
            archive_dir.mkdir()
            archive = archive_dir / f"base_3-{digest}.c"
            archive.write_bytes(content)
            entry = parse_match_log_line(
                f".matches/base_3-{digest}.c 100.000% "
                f"exact sha256={digest} source=base_3.c"
            )

            self.assertIsNotNone(entry)
            assert entry is not None
            self.assertEqual(certified_attempt_path(entry, workspace), archive)

    def test_modified_archive_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temp:
            workspace = Path(temp)
            original = b"matching source\n"
            digest = hashlib.sha256(original).hexdigest()
            archive_dir = workspace / ".matches"
            archive_dir.mkdir()
            archive = archive_dir / f"base_3-{digest}.c"
            archive.write_bytes(b"modified\n")
            entry = parse_match_log_line(
                f".matches/base_3-{digest}.c 100.000% exact sha256={digest}"
            )

            self.assertIsNotNone(entry)
            assert entry is not None
            self.assertIsNone(certified_attempt_path(entry, workspace))

    def test_full_match_outside_archive_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temp:
            workspace = Path(temp)
            content = b"matching source\n"
            digest = hashlib.sha256(content).hexdigest()
            attempt = workspace / f"base_3-{digest}.c"
            attempt.write_bytes(content)
            entry = parse_match_log_line(
                f"base_3-{digest}.c 100.000% exact sha256={digest}"
            )

            self.assertIsNotNone(entry)
            assert entry is not None
            self.assertIsNone(certified_attempt_path(entry, workspace))


class DistanceOutputTest(unittest.TestCase):
    def run_dist(
        self, target: str, candidate: str, algorithm: str = "sequence"
    ) -> str:
        with tempfile.TemporaryDirectory() as temp:
            temp_dir = Path(temp)
            target_path = temp_dir / "target.txt"
            candidate_path = temp_dir / "candidate.txt"
            target_path.write_text(target)
            candidate_path.write_text(candidate)
            return subprocess.check_output(
                [
                    sys.executable,
                    str(DIST),
                    str(target_path),
                    str(candidate_path),
                    "--algorithm",
                    algorithm,
                ],
                text=True,
            )

    def test_exact_input_is_explicitly_exact(self) -> None:
        output = self.run_dist("addiu v0,v0,1\n", "addiu v0,v0,1\n")

        self.assertIn("Score: 100.000% (0 differences)", output)
        self.assertIn("Algorithm: sequence", output)
        self.assertIn("Exact match: yes", output)

    def test_levenshtein_algorithm_is_reported(self) -> None:
        output = self.run_dist(
            "addiu v0,v0,1\n",
            "addiu v0,v0,1\n",
            algorithm="levenshtein",
        )

        self.assertIn("Score: 100.000% (0 differences)", output)
        self.assertIn("Algorithm: levenshtein", output)
        self.assertIn("Exact match: yes", output)

    def test_operand_difference_never_rounds_to_full_match(self) -> None:
        output = self.run_dist("addiu v0,v0,1\n", "addiu v1,v0,1\n")

        self.assertNotIn("Score: 100.000%", output)
        self.assertIn("Exact match: no", output)


if __name__ == "__main__":
    unittest.main()
