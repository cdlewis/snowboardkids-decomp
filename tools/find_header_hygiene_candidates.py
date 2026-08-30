#!/usr/bin/env python3
"""Find project headers/source pairs that violate header ownership rules."""

import argparse
import json
import re
from collections import defaultdict
from pathlib import Path


HEADER_ROOT = Path("include/game")
SOURCE_ROOT = Path("src")
EXCLUDED_SOURCE_ROOTS = (Path("src/ultra"),)
GENERATED_CONTEXT_NAME = "c" + "tx.c"

FORWARD_TYPE_RE = re.compile(
    r"\btypedef\s+(struct|union)\s+([A-Za-z_]\w*)\s+\2\s*;"
)
EXTERN_FUNCTION_RE = re.compile(
    r"^[ \t]*extern[ \t]+[^;={}]*?\b([A-Za-z_]\w*)[ \t\r\n]*"
    r"\([^;{}]*\)[ \t\r\n]*;",
    re.MULTILINE,
)
INCLUDE_RE = re.compile(r'^\s*#\s*include\s+"([^"]+\.h)"', re.MULTILINE)


def has_complete_definition(text: str, kind: str, name: str) -> bool:
    definition = re.compile(
        rf"\b(?:typedef\s+)?{kind}\s+{re.escape(name)}\s*\{{"
    )
    return definition.search(text) is not None


def is_excluded_source(path: Path) -> bool:
    return path.name == GENERATED_CONTEXT_NAME or any(
        path == root or root in path.parents for root in EXCLUDED_SOURCE_ROOTS
    )


def matching_headers(source_path: Path, source: str) -> list[Path]:
    headers = []
    likely_header = HEADER_ROOT / source_path.relative_to(SOURCE_ROOT).with_suffix(".h")
    if likely_header.is_file():
        headers.append(likely_header)
    for include_path in INCLUDE_RE.findall(source):
        header_path = Path("include") / include_path
        if header_path.is_file() and Path(include_path).stem == source_path.stem:
            headers.append(header_path)
    return sorted(set(headers), key=str)


def find_candidates(stable: bool = False) -> list:
    issues: dict[tuple[str, str], set[str]] = defaultdict(set)

    for header_path in sorted(HEADER_ROOT.rglob("*.h"), key=str):
        text = header_path.read_text()
        for kind, name in FORWARD_TYPE_RE.findall(text):
            if not has_complete_definition(text, kind, name):
                issues[(str(header_path), "")].add(f"INCOMPLETE_TYPE:{name}")

        for name in EXTERN_FUNCTION_RE.findall(text):
            issues[(str(header_path), "")].add(f"EXTERN_FUNCTION_IN_HEADER:{name}")

    for source_path in sorted(SOURCE_ROOT.rglob("*.c"), key=str):
        if is_excluded_source(source_path):
            continue
        text = source_path.read_text()
        function_names = EXTERN_FUNCTION_RE.findall(text)
        if not function_names:
            continue

        headers = matching_headers(source_path, text)
        if headers:
            for header_path in headers:
                key = (str(header_path), str(source_path))
                issues[key].update(
                    f"SOURCE_EXTERN_FUNCTION:{name}" for name in function_names
                )
        else:
            key = ("NO_OWNING_HEADER", str(source_path))
            issues[key].update(
                f"SOURCE_EXTERN_FUNCTION:{name}" for name in function_names
            )

    # Merge header-only findings into a source/header candidate when possible.
    merged: dict[tuple[str, str], set[str]] = defaultdict(set)
    for (header, source), problem_set in issues.items():
        if source or header == "NO_OWNING_HEADER":
            merged[(header, source)].update(problem_set)
            continue

        header_path = Path(header)
        relative = header_path.relative_to(HEADER_ROOT)
        likely_source = SOURCE_ROOT / relative.with_suffix(".c")
        key = (header, str(likely_source) if likely_source.is_file() else "")
        merged[key].update(problem_set)

    candidates = []
    for (header, source), problem_set in sorted(merged.items()):
        if stable:
            # Nigel uses the entire JSON value as the candidate identity. Keep
            # that identity path-based so fixing one of several reported
            # problems cannot make a still-problematic candidate look done.
            candidates.append([header, source])
            continue
        paths = " ".join(path for path in (header, source) if path)
        candidates.append(f"{paths} [{' '.join(sorted(problem_set))}]")
    return candidates


def main() -> None:
    parser = argparse.ArgumentParser(
        description="List header-hygiene candidates for task-runner"
    )
    parser.add_argument(
        "--json", action="store_true", help="emit a JSON array for task-runner"
    )
    args = parser.parse_args()

    if args.json:
        print(json.dumps(find_candidates(stable=True)))
    else:
        print("\n".join(find_candidates()))


if __name__ == "__main__":
    main()
