#!/usr/bin/env python3
"""Rank unmatched functions by their overall decompilation leverage.

For each unmatched function, this script asks two counterfactual questions:

* How much ROM progress would matching the function make directly?
* How much easier would it make other unmatched functions by becoming a new
  decompile-similar reference?

The default "overall" ranking estimates value per unit of effort. Value includes
the candidate's own instructions plus affected targets, weighted by target size
and the improvement in similarity. Effort accounts for candidate size,
structural difficulty, the quality of its current reference, and prior failed
task-runner attempts.

Usage:
    python3 tools/rank_decompile_similar_impact.py
    python3 tools/rank_decompile_similar_impact.py --limit 50 --show-targets 5
    python3 tools/rank_decompile_similar_impact.py --ranking reach
    python3 tools/rank_decompile_similar_impact.py --json
"""

from __future__ import annotations

import argparse
import json
import os
import sys
from dataclasses import dataclass, field
from pathlib import Path
from statistics import mean
from typing import Iterable, Sequence

from find_similar_functions import (
    ParsedFunction,
    build_function_index,
    calculate_similarity,
)
from score_functions import (
    decompilation_difficulty_score,
    get_similarity_worktrees,
    worktree_label_for_function,
)

DEFAULT_UNMATCHED_DIR = "asm/nonmatchings"
DEFAULT_IGNORED_LOG = "task-runner/decompile-similar/ignored.log"


@dataclass(frozen=True)
class Reference:
    """One similarity recommendation for an unmatched target."""

    name: str
    label: str
    score: float

    @property
    def task_label(self) -> str:
        """Match score_functions.py's task-runner JSON representation."""
        return f"{self.label} ({self.score:.0%})"


@dataclass(frozen=True)
class TargetImpact:
    """How one hypothetical match changes one target's recommendations."""

    target: str
    score: float
    new_rank: int
    replaced_score: float
    unblocked: bool
    instruction_count: int = 0

    @property
    def score_gain(self) -> float:
        return max(0.0, self.score - self.replaced_score)

    @property
    def effective_score_gain(self) -> float:
        """Hint value newly available after matching the candidate.

        A normal target gets only the improvement over its current hint. If the
        current task input was already exhausted, the replacement creates a
        fresh attempt, so its full similarity is useful again.
        """
        return self.score if self.unblocked else self.score_gain

    @property
    def value(self) -> float:
        return self.instruction_count * self.effective_score_gain


@dataclass
class CandidateImpact:
    """Aggregated impact of matching one currently unmatched function."""

    function: ParsedFunction
    candidate_blocked: bool
    reference_similarity: float = 0.0
    historical_attempts: int = 0
    targets: list[TargetImpact] = field(default_factory=list)

    @property
    def changed_count(self) -> int:
        return len(self.targets)

    @property
    def unblocked_count(self) -> int:
        return sum(target.unblocked for target in self.targets)

    @property
    def new_best_count(self) -> int:
        return sum(target.new_rank == 1 for target in self.targets)

    @property
    def total_score_gain(self) -> float:
        return sum(target.score_gain for target in self.targets)

    @property
    def average_similarity(self) -> float:
        if not self.targets:
            return 0.0
        return mean(target.score for target in self.targets)

    @property
    def difficulty(self) -> float:
        return decompilation_difficulty_score(
            self.function.instruction_count,
            self.function.branch_count,
            self.function.jump_count,
            self.function.label_count,
        )

    @property
    def direct_value(self) -> float:
        return float(self.function.instruction_count)

    @property
    def downstream_value(self) -> float:
        return sum(target.value for target in self.targets)

    @property
    def total_value(self) -> float:
        return self.direct_value + self.downstream_value

    @property
    def estimated_effort(self) -> float:
        """Estimate relative effort in instruction-equivalent units.

        The fixed cost represents setup/integration overhead, while the other
        factors deliberately have modest ranges so no single heuristic can
        dominate the measured function size.
        """
        fixed_overhead = 25.0
        difficulty_factor = 1.0 + self.difficulty
        reference_factor = 1.25 - (0.5 * self.reference_similarity)
        retry_factor = 1.0 + (0.25 * self.historical_attempts)
        return fixed_overhead + (
            max(1, self.function.instruction_count)
            * difficulty_factor
            * reference_factor
            * retry_factor
        )

    @property
    def overall_score(self) -> float:
        return self.total_value / self.estimated_effort


def reference_sort_key(reference: Reference) -> tuple[float, bool, str, str]:
    """Mirror score_functions.find_similar_cross_worktree ordering."""
    return (
        -reference.score,
        reference.label != reference.name,
        reference.label,
        reference.name,
    )


def select_top_references(
    references: Iterable[Reference], top_n: int
) -> list[Reference]:
    """Sort and deduplicate recommendations using decompile-similar rules."""
    selected = []
    seen_names = set()

    for reference in sorted(references, key=reference_sort_key):
        if reference.name in seen_names:
            continue
        seen_names.add(reference.name)
        selected.append(reference)
        if len(selected) == top_n:
            break

    return selected


def add_hypothetical_reference(
    current: Sequence[Reference], candidate: Reference, top_n: int
) -> list[Reference]:
    """Return the recommendation list after hypothetically matching candidate."""
    return select_top_references([*current, candidate], top_n)


def load_ignored_inputs(path: Path) -> set[tuple[str, ...]]:
    """Read exact task inputs recorded by task-runner, one JSON array per line."""
    if not path.is_file():
        return set()

    ignored = set()
    malformed_lines = []

    with path.open() as ignored_file:
        for line_number, line in enumerate(ignored_file, 1):
            line = line.strip()
            if not line:
                continue
            try:
                value = json.loads(line)
            except json.JSONDecodeError:
                malformed_lines.append(line_number)
                continue
            if not isinstance(value, list) or not all(
                isinstance(item, str) for item in value
            ):
                malformed_lines.append(line_number)
                continue
            ignored.add(tuple(value))

    if malformed_lines:
        lines = ", ".join(str(line) for line in malformed_lines[:5])
        suffix = "..." if len(malformed_lines) > 5 else ""
        print(
            f"Warning: ignored malformed lines in {path}: {lines}{suffix}",
            file=sys.stderr,
        )

    return ignored


def function_fingerprint(function: ParsedFunction) -> tuple:
    """Return all parsed features that can affect calculate_similarity."""
    return (
        tuple(function.normalized_instructions),
        function.instruction_count,
        function.branch_count,
        function.jump_count,
        function.label_count,
        function.stack_size,
        function.control_flow_signature,
        tuple(function.data_access_offsets),
        frozenset(function.instruction_ngrams),
    )


def preferred_function_key(
    function: ParsedFunction, project_root: str, worktrees: Sequence[Path]
) -> tuple[bool, str, str]:
    label = worktree_label_for_function(function, project_root, list(worktrees))
    return label != function.name, label, function.file_path


def deduplicate_matching_index(
    functions: Iterable[ParsedFunction],
    project_root: str,
    worktrees: Sequence[Path],
) -> list[ParsedFunction]:
    """Remove identical cross-worktree copies without changing recommendations.

    A function name can retain multiple entries only if its parsed assembly
    differs between worktrees. In that unusual case the normal similarity
    ranking decides which version represents the name for each target.
    """
    best_by_identity: dict[tuple[str, tuple], ParsedFunction] = {}

    for function in functions:
        key = function.name, function_fingerprint(function)
        current = best_by_identity.get(key)
        if current is None or preferred_function_key(
            function, project_root, worktrees
        ) < preferred_function_key(current, project_root, worktrees):
            best_by_identity[key] = function

    return list(best_by_identity.values())


def build_current_references(
    unmatched: Sequence[ParsedFunction],
    matching: Sequence[ParsedFunction],
    project_root: str,
    worktrees: Sequence[Path],
    top_n: int,
) -> dict[str, list[Reference]]:
    """Compute the exact current top-N recommendations for every target."""
    labels = {
        function.file_path: worktree_label_for_function(
            function, project_root, list(worktrees)
        )
        for function in matching
    }
    recommendations = {}

    for index, query in enumerate(unmatched, 1):
        references = []
        query_path = Path(query.file_path).resolve()

        for candidate in matching:
            if Path(candidate.file_path).resolve() == query_path:
                continue
            result = calculate_similarity(query, candidate)
            references.append(
                Reference(
                    name=candidate.name,
                    label=labels[candidate.file_path],
                    score=result.total_score,
                )
            )

        recommendations[query.name] = select_top_references(references, top_n)
        if index % 25 == 0 or index == len(unmatched):
            print(
                f"  Current recommendations: {index}/{len(unmatched)}",
                file=sys.stderr,
            )

    return recommendations


def task_input(target: str, references: Sequence[Reference]) -> tuple[str, ...]:
    return target, *(reference.task_label for reference in references)


def rank_candidate_impacts(
    unmatched: Sequence[ParsedFunction],
    current_references: dict[str, list[Reference]],
    matched_names: set[str],
    ignored_inputs: set[tuple[str, ...]],
    top_n: int,
    ranking: str = "overall",
    show_progress: bool = True,
) -> tuple[list[CandidateImpact], set[str]]:
    """Calculate recommendation changes caused by every possible new match."""
    current_inputs = {
        target.name: task_input(target.name, current_references[target.name])
        for target in unmatched
    }
    blocked_targets = {
        name
        for name, current_input in current_inputs.items()
        if current_input in ignored_inputs
    }
    historical_attempts: dict[str, int] = {}
    for ignored_input in ignored_inputs:
        if ignored_input:
            historical_attempts[ignored_input[0]] = (
                historical_attempts.get(ignored_input[0], 0) + 1
            )
    impacts = [
        CandidateImpact(
            function=candidate,
            candidate_blocked=candidate.name in blocked_targets,
            reference_similarity=(
                current_references[candidate.name][0].score
                if current_references[candidate.name]
                else 0.0
            ),
            historical_attempts=historical_attempts.get(candidate.name, 0),
        )
        for candidate in unmatched
        if candidate.name not in matched_names
    ]

    for index, impact in enumerate(impacts, 1):
        candidate = impact.function

        for target in unmatched:
            if target.name == candidate.name:
                continue

            result = calculate_similarity(target, candidate)
            hypothetical = Reference(
                name=candidate.name,
                label=candidate.name,
                score=result.total_score,
            )
            current = current_references[target.name]
            updated = add_hypothetical_reference(current, hypothetical, top_n)

            if [reference.label for reference in updated] == [
                reference.label for reference in current
            ]:
                continue

            new_rank = next(
                rank
                for rank, reference in enumerate(updated, 1)
                if reference.name == candidate.name
            )
            replaced_score = current[-1].score if len(current) == top_n else 0.0
            old_input = current_inputs[target.name]
            new_input = task_input(target.name, updated)
            impact.targets.append(
                TargetImpact(
                    target=target.name,
                    score=result.total_score,
                    new_rank=new_rank,
                    replaced_score=replaced_score,
                    unblocked=(
                        old_input in ignored_inputs and new_input not in ignored_inputs
                    ),
                    instruction_count=target.instruction_count,
                )
            )

        if show_progress and (index % 25 == 0 or index == len(impacts)):
            print(
                f"  Hypothetical matches: {index}/{len(impacts)}",
                file=sys.stderr,
            )

    if ranking == "overall":
        impacts.sort(
            key=lambda impact: (
                -impact.overall_score,
                -impact.total_value,
                impact.estimated_effort,
                impact.function.name,
            )
        )
    elif ranking == "reach":
        impacts.sort(
            key=lambda impact: (
                -impact.unblocked_count,
                -impact.changed_count,
                -impact.new_best_count,
                -impact.total_score_gain,
                impact.difficulty,
                impact.function.instruction_count,
                impact.function.name,
            )
        )
    else:
        raise ValueError(f"unknown ranking mode: {ranking}")

    return impacts, blocked_targets


def impact_to_json(impact: CandidateImpact) -> dict:
    targets = sorted(
        impact.targets,
        key=lambda target: (
            not target.unblocked,
            target.new_rank,
            -target.score,
            target.target,
        ),
    )
    return {
        "function": impact.function.name,
        "candidate_blocked": impact.candidate_blocked,
        "historical_attempts": impact.historical_attempts,
        "reference_similarity": impact.reference_similarity,
        "overall_score": impact.overall_score,
        "direct_value": impact.direct_value,
        "downstream_value": impact.downstream_value,
        "total_value": impact.total_value,
        "estimated_effort": impact.estimated_effort,
        "unblocked_targets": impact.unblocked_count,
        "changed_targets": impact.changed_count,
        "new_best_targets": impact.new_best_count,
        "total_score_gain": impact.total_score_gain,
        "average_similarity": impact.average_similarity,
        "instruction_count": impact.function.instruction_count,
        "difficulty": impact.difficulty,
        "targets": [
            {
                "function": target.target,
                "similarity": target.score,
                "new_rank": target.new_rank,
                "replaced_score": target.replaced_score,
                "score_gain": target.score_gain,
                "effective_score_gain": target.effective_score_gain,
                "instruction_count": target.instruction_count,
                "value": target.value,
                "unblocked": target.unblocked,
            }
            for target in targets
        ],
    }


def print_human_results(
    impacts: Sequence[CandidateImpact],
    unmatched_count: int,
    matching_count: int,
    blocked_targets: set[str],
    ignored_input_count: int,
    excluded_existing_count: int,
    top_n: int,
    show_targets: int,
    ranking: str,
) -> None:
    print(
        f"Analyzed {unmatched_count} unmatched functions against "
        f"{matching_count} unique matched references (top {top_n})."
    )
    if blocked_targets:
        print(
            f"Current decompile-similar inputs already ignored: "
            f"{len(blocked_targets)}/{unmatched_count}."
        )
    elif ignored_input_count:
        print(
            f"Loaded {ignored_input_count} historical ignored inputs; none exactly "
            "match the current hint lists."
        )
    if excluded_existing_count:
        print(
            f"Excluded {excluded_existing_count} unmatched functions already available "
            "as matched references in another worktree."
        )
    print()
    if ranking == "overall":
        print(
            f"{'#':>3}  {'FUNCTION':<42} {'SCORE':>6} {'VALUE':>7} "
            f"{'EFFORT':>7} {'STATE':<7} {'REF':>5} {'TRIES':>5} "
            f"{'CHANGE':>6}  "
            "TARGET EXAMPLES"
        )
    else:
        print(
            f"{'#':>3}  {'FUNCTION':<42} {'STATE':<7} {'UNBLOCK':>7} "
            f"{'CHANGE':>6} {'BEST':>4} {'INSNS':>5} {'AVG SIM':>7}  "
            "TARGET EXAMPLES"
        )

    for rank, impact in enumerate(impacts, 1):
        targets = sorted(
            impact.targets,
            key=lambda target: (
                not target.unblocked,
                target.new_rank,
                -target.score,
                target.target,
            ),
        )
        examples = []
        for target in targets[:show_targets]:
            marker = "*" if target.unblocked else ""
            examples.append(
                f"{target.target}@{target.score:.0%}/#{target.new_rank}{marker}"
            )
        if ranking == "overall":
            print(
                f"{rank:>3}  {impact.function.name:<42} "
                f"{impact.overall_score:>6.2f} {impact.total_value:>7.1f} "
                f"{impact.estimated_effort:>7.1f} "
                f"{'blocked' if impact.candidate_blocked else 'ready':<7} "
                f"{impact.reference_similarity:>5.0%} "
                f"{impact.historical_attempts:>5} {impact.changed_count:>6}  "
                f"{', '.join(examples)}"
            )
        else:
            print(
                f"{rank:>3}  {impact.function.name:<42} "
                f"{'blocked' if impact.candidate_blocked else 'ready':<7} "
                f"{impact.unblocked_count:>7} {impact.changed_count:>6} "
                f"{impact.new_best_count:>4} "
                f"{impact.function.instruction_count:>5} "
                f"{impact.average_similarity:>7.1%}  {', '.join(examples)}"
            )

    if ranking == "overall":
        print(
            "\nScore = value / effort. Value is direct instructions plus "
            "similarity-weighted downstream instructions."
        )
        print(
            "Effort estimates setup, size, structural difficulty, current "
            "reference quality, and prior tries."
        )
    if any(impact.unblocked_count for impact in impacts):
        print("\n* target gets a task input that is not already in ignored.log")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Rank unmatched functions by overall decompilation value per "
            "estimated effort, including their impact as future references."
        )
    )
    parser.add_argument(
        "unmatched_dir",
        nargs="?",
        default=DEFAULT_UNMATCHED_DIR,
        help=f"unmatched assembly directory (default: {DEFAULT_UNMATCHED_DIR})",
    )
    parser.add_argument(
        "--top-n",
        type=int,
        default=1,
        metavar="N",
        help="number of hints used by decompile-similar (default: 1)",
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=25,
        metavar="N",
        help="number of candidates to print; 0 prints all (default: 25)",
    )
    parser.add_argument(
        "--show-targets",
        type=int,
        default=3,
        metavar="N",
        help="example affected targets per candidate (default: 3)",
    )
    parser.add_argument(
        "--ignored-log",
        type=Path,
        default=Path(DEFAULT_IGNORED_LOG),
        help=f"task-runner ignore history (default: {DEFAULT_IGNORED_LOG})",
    )
    parser.add_argument(
        "--ranking",
        choices=("overall", "reach"),
        default="overall",
        help=(
            "ranking model: overall value per estimated effort (default), or "
            "legacy reach-first ordering"
        ),
    )
    candidate_filter = parser.add_mutually_exclusive_group()
    candidate_filter.add_argument(
        "--blocked-candidates-only",
        action="store_true",
        help="show only candidates whose exact current task input was exhausted",
    )
    candidate_filter.add_argument(
        "--all-candidates",
        action="store_true",
        help=argparse.SUPPRESS,
    )
    parser.add_argument(
        "--include-zero-impact",
        action="store_true",
        help=(
            "in reach mode, include functions that would not change any top-N "
            "hint list (overall mode always includes their direct value)"
        ),
    )
    parser.add_argument(
        "--json",
        action="store_true",
        help="write complete machine-readable results to stdout",
    )
    args = parser.parse_args()

    if args.top_n < 1:
        parser.error("--top-n must be at least 1")
    if args.limit < 0:
        parser.error("--limit cannot be negative")
    if args.show_targets < 0:
        parser.error("--show-targets cannot be negative")
    return args


def main() -> int:
    args = parse_args()
    project_root = str(Path(__file__).resolve().parent.parent)
    os.chdir(project_root)

    unmatched_dir = Path(args.unmatched_dir)
    if not unmatched_dir.is_dir():
        print(
            f"Error: unmatched directory does not exist: {unmatched_dir}",
            file=sys.stderr,
        )
        return 1

    print(f"Parsing unmatched functions from {unmatched_dir}...", file=sys.stderr)
    unmatched = build_function_index(str(unmatched_dir))
    unmatched.sort(key=lambda function: function.name)
    if not unmatched:
        print(
            f"Error: no unmatched functions found in {unmatched_dir}", file=sys.stderr
        )
        return 1

    worktrees = get_similarity_worktrees(project_root)
    worktree_list = ", ".join(os.path.relpath(path, project_root) for path in worktrees)
    print(f"Parsing matched references from {worktree_list}...", file=sys.stderr)
    raw_matching = []
    for worktree in worktrees:
        raw_matching.extend(build_function_index(str(worktree / "asm" / "matchings")))
    matching = deduplicate_matching_index(raw_matching, project_root, worktrees)
    matched_names = {function.name for function in matching}
    print(
        f"Indexed {len(matching)} distinct matched references "
        f"({len(raw_matching)} including worktree copies).",
        file=sys.stderr,
    )

    current = build_current_references(
        unmatched, matching, project_root, worktrees, args.top_n
    )
    ignored_inputs = load_ignored_inputs(args.ignored_log)
    impacts, blocked_targets = rank_candidate_impacts(
        unmatched,
        current,
        matched_names,
        ignored_inputs,
        args.top_n,
        ranking=args.ranking,
    )

    excluded_existing_count = sum(
        function.name in matched_names for function in unmatched
    )
    if args.blocked_candidates_only:
        impacts = [impact for impact in impacts if impact.candidate_blocked]
    if args.ranking == "reach" and not args.include_zero_impact:
        impacts = [impact for impact in impacts if impact.changed_count]
    if args.limit:
        impacts = impacts[: args.limit]

    if args.json:
        result = {
            "metadata": {
                "unmatched_functions": len(unmatched),
                "matched_references": len(matching),
                "top_n": args.top_n,
                "ignored_inputs": len(ignored_inputs),
                "currently_blocked_targets": len(blocked_targets),
                "excluded_existing_references": excluded_existing_count,
                "ranking": args.ranking,
            },
            "candidates": [impact_to_json(impact) for impact in impacts],
        }
        print(json.dumps(result, indent=2))
    else:
        print_human_results(
            impacts,
            len(unmatched),
            len(matching),
            blocked_targets,
            len(ignored_inputs),
            excluded_existing_count,
            args.top_n,
            args.show_targets,
            args.ranking,
        )

    return 0


if __name__ == "__main__":
    sys.exit(main())
