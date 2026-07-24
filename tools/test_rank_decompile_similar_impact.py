#!/usr/bin/env python3
"""Tests for rank_decompile_similar_impact.py."""

import json
import tempfile
import unittest
from pathlib import Path

from rank_decompile_similar_impact import (
    CandidateImpact,
    Reference,
    TargetImpact,
    add_hypothetical_reference,
    load_ignored_inputs,
    rank_candidate_impacts,
    select_top_references,
    task_input,
)
from find_similar_functions import ParsedFunction


class RecommendationTests(unittest.TestCase):
    def test_hypothetical_reference_displaces_last_hint(self):
        current = [
            Reference("one", "one", 0.9),
            Reference("two", "two", 0.8),
        ]

        updated = add_hypothetical_reference(
            current, Reference("new", "new", 0.85), top_n=2
        )

        self.assertEqual([reference.name for reference in updated], ["one", "new"])

    def test_hypothetical_reference_below_cutoff_has_no_effect(self):
        current = [
            Reference("one", "one", 0.9),
            Reference("two", "two", 0.8),
        ]

        updated = add_hypothetical_reference(
            current, Reference("new", "new", 0.7), top_n=2
        )

        self.assertEqual(updated, current)

    def test_active_worktree_wins_score_tie(self):
        references = [
            Reference("same", "same (../sbk-a)", 0.9),
            Reference("same", "same", 0.9),
        ]

        selected = select_top_references(references, top_n=1)

        self.assertEqual(selected[0].label, "same")

    def test_ignored_log_uses_exact_task_input(self):
        task = task_input("target", [Reference("hint", "hint", 0.9)])
        with tempfile.TemporaryDirectory() as temp_dir:
            path = Path(temp_dir) / "ignored.log"
            path.write_text(json.dumps(list(task)) + "\n")

            ignored = load_ignored_inputs(path)

        self.assertIn(task, ignored)
        self.assertNotIn(("target", "different_hint"), ignored)

    def test_task_input_matches_score_functions_percentage_format(self):
        task = task_input(
            "target",
            [Reference("hint", "hint (../sbk-a)", 0.643)],
        )

        self.assertEqual(task, ("target", "hint (../sbk-a) (64%)"))

    def test_new_hint_unblocks_previously_ignored_target(self):
        target = ParsedFunction(
            name="target",
            file_path="target.s",
            normalized_instructions=["addiu", "addu", "jr"],
            instruction_count=3,
        )
        candidate = ParsedFunction(
            name="candidate",
            file_path="candidate.s",
            normalized_instructions=["addiu", "addu", "jr"],
            instruction_count=3,
        )
        old_reference = Reference("old", "old", 0.0)
        recommendations = {
            "target": [old_reference],
            "candidate": [old_reference],
        }
        ignored = {task_input("target", [old_reference])}

        impacts, blocked_targets = rank_candidate_impacts(
            [target, candidate],
            recommendations,
            matched_names=set(),
            ignored_inputs=ignored,
            top_n=1,
            show_progress=False,
        )

        candidate_impact = next(
            impact for impact in impacts if impact.function.name == "candidate"
        )
        self.assertEqual(blocked_targets, {"target"})
        self.assertEqual(candidate_impact.changed_count, 1)
        self.assertEqual(candidate_impact.unblocked_count, 1)

    def test_overall_score_rewards_strong_downstream_reference(self):
        candidate = ParsedFunction(
            name="candidate",
            file_path="candidate.s",
            instruction_count=100,
        )
        low_value = CandidateImpact(
            function=candidate,
            candidate_blocked=False,
            targets=[
                TargetImpact(
                    target="low",
                    score=0.51,
                    new_rank=1,
                    replaced_score=0.50,
                    unblocked=False,
                    instruction_count=500,
                )
            ],
        )
        high_value = CandidateImpact(
            function=candidate,
            candidate_blocked=False,
            targets=[
                TargetImpact(
                    target="high",
                    score=0.90,
                    new_rank=1,
                    replaced_score=0.50,
                    unblocked=False,
                    instruction_count=500,
                )
            ],
        )

        self.assertGreater(high_value.overall_score, low_value.overall_score)

    def test_unblocked_target_recovers_full_hint_value(self):
        impact = TargetImpact(
            target="target",
            score=0.80,
            new_rank=1,
            replaced_score=0.75,
            unblocked=True,
            instruction_count=100,
        )

        self.assertAlmostEqual(impact.score_gain, 0.05)
        self.assertEqual(impact.effective_score_gain, 0.80)
        self.assertEqual(impact.value, 80.0)

    def test_prior_attempts_increase_estimated_effort(self):
        function = ParsedFunction(
            name="candidate",
            file_path="candidate.s",
            instruction_count=100,
        )
        fresh = CandidateImpact(function=function, candidate_blocked=False)
        retried = CandidateImpact(
            function=function,
            candidate_blocked=False,
            historical_attempts=2,
        )

        self.assertGreater(retried.estimated_effort, fresh.estimated_effort)
        self.assertLess(retried.overall_score, fresh.overall_score)


if __name__ == "__main__":
    unittest.main()
