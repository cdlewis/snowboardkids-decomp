#!/usr/bin/env python3
"""Replace source-local function externs with their declaring headers.

The checker indexes public project function prototypes below ``include/game/``
and then looks for matching ``extern`` function declarations in C source
files.  By default it only reports violations and exits non-zero.  Pass
``--fix`` to remove the redundant declarations and add the appropriate direct
``#include``.  Pass ``--all-headers`` to include internal and vendored SDK
headers in the audit.

The fixer deliberately leaves a declaration alone when two equally suitable
headers declare the same function.  In that case the report lists the
candidate headers so a developer can resolve the ambiguity explicitly.
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Sequence


IDENTIFIER = re.compile(r"\b([A-Za-z_][A-Za-z0-9_]*)\s*\(")
INCLUDE = re.compile(r'^[ \t]*#[ \t]*include[ \t]+[<"]([^">]+)[">]', re.MULTILINE)
FUNCTION_DEFINITION = re.compile(
    r"^[ \t]*(?!(?:if|for|while|switch)\b)"
    r"(?:[A-Za-z_][A-Za-z0-9_]*[ \t*]+)+"
    r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*"
    r"\([^;{}]*\)\s*\{",
    re.MULTILINE,
)
HEADER_PREREQUISITES = {
    "PR/os_system.h": ("PR/os_exception.h",),
}


@dataclass(frozen=True)
class Declaration:
    path: Path
    start: int
    end: int
    line: int
    name: str
    text: str


@dataclass(frozen=True)
class Violation:
    declaration: Declaration
    candidates: tuple[Path, ...]
    selected_header: Path | None
    same_file_forward: bool = False


def mask_comments_and_literals(text: str) -> str:
    """Blank comments and literals while preserving offsets and newlines."""
    result = list(text)
    index = 0
    state = "code"

    while index < len(text):
        char = text[index]
        next_char = text[index + 1] if index + 1 < len(text) else ""

        if state == "code":
            if char == "/" and next_char == "/":
                result[index] = result[index + 1] = " "
                index += 2
                state = "line_comment"
                continue
            if char == "/" and next_char == "*":
                result[index] = result[index + 1] = " "
                index += 2
                state = "block_comment"
                continue
            if char == '"':
                result[index] = " "
                index += 1
                state = "string"
                continue
            if char == "'":
                result[index] = " "
                index += 1
                state = "character"
                continue
        elif state == "line_comment":
            if char == "\n":
                state = "code"
            else:
                result[index] = " "
            index += 1
            continue
        elif state == "block_comment":
            if char == "*" and next_char == "/":
                result[index] = result[index + 1] = " "
                index += 2
                state = "code"
                continue
            if char != "\n":
                result[index] = " "
            index += 1
            continue
        else:
            if char == "\\" and next_char:
                if char != "\n":
                    result[index] = " "
                if next_char != "\n":
                    result[index + 1] = " "
                index += 2
                continue
            if (state == "string" and char == '"') or (
                state == "character" and char == "'"
            ):
                result[index] = " "
                index += 1
                state = "code"
                continue
            if char != "\n":
                result[index] = " "
            index += 1
            continue

        index += 1

    return "".join(result)


def function_name(statement: str) -> str | None:
    stripped = statement.lstrip()
    if stripped.startswith(("typedef ", "static ", "#")) or "=" in statement:
        return None

    match = IDENTIFIER.search(statement)
    if match is None:
        return None
    function_pointer = re.search(r"\(\s*\*[^)]*\)\s*\(", statement)
    if function_pointer is not None and function_pointer.start() < match.start():
        return None
    return match.group(1)


def find_declarations(path: Path, source_externs: bool) -> list[Declaration]:
    text = path.read_text()
    masked = mask_comments_and_literals(text)
    declarations = []
    lines = masked.splitlines(keepends=True)
    original_lines = text.splitlines(keepends=True)
    offset = 0
    brace_stack: list[str] = []
    pending_start: int | None = None
    pending_line = 0
    pending_parts: list[str] = []

    for line_number, (line, original_line) in enumerate(
        zip(lines, original_lines), 1
    ):
        stripped = line.lstrip()
        continued_statement = pending_start is not None
        at_declaration_scope = not brace_stack or (
            not source_externs and all(kind == "extern_c" for kind in brace_stack)
        )

        if pending_start is not None:
            if (
                not at_declaration_scope
                or "#" in stripped
                or "{" in line
                or "}" in line
            ):
                pending_start = None
                pending_parts = []
            else:
                pending_parts.append(line)
                if ";" in line:
                    statement = "".join(pending_parts)
                    statement = statement[: statement.index(";") + 1]
                    name = function_name(statement)
                    if name is not None:
                        end = pending_start + len(statement)
                        if not masked[end : offset + len(line)].strip():
                            end = offset + len(line)
                        declarations.append(
                            Declaration(
                                path=path,
                                start=pending_start,
                                end=end,
                                line=pending_line,
                                name=name,
                                text=text[pending_start:end],
                            )
                        )
                    pending_start = None
                    pending_parts = []

        if pending_start is None and not continued_statement and at_declaration_scope:
            starts_extern = re.match(r"extern\b", stripped) is not None
            starts_header_statement = (
                bool(stripped)
                and (stripped[0].isalpha() or stripped[0] == "_")
                and not stripped.startswith("#")
            )
            if (source_externs and starts_extern) or (
                not source_externs and starts_header_statement
            ):
                if "{" not in line and "}" not in line:
                    pending_start = offset
                    pending_line = line_number
                    pending_parts = [line]
                    if ";" in line:
                        statement = line[: line.index(";") + 1]
                        name = function_name(statement)
                        if name is not None:
                            end = offset + len(statement)
                            if not masked[end : offset + len(line)].strip():
                                end = offset + len(line)
                            declarations.append(
                                Declaration(
                                    path=path,
                                    start=offset,
                                    end=end,
                                    line=line_number,
                                    name=name,
                                    text=text[offset:end],
                                )
                            )
                        pending_start = None
                        pending_parts = []

        for match in re.finditer(r"[{}]", line):
            if match.group() == "{":
                original_prefix = original_line[: match.start()]
                kind = (
                    "extern_c"
                    if re.search(r'\bextern\s*"C"\s*$', original_prefix)
                    else "other"
                )
                brace_stack.append(kind)
            elif brace_stack:
                brace_stack.pop()
        offset += len(line)
    return declarations


def declaration_signature(
    declaration: Declaration,
) -> tuple[str, tuple[str, ...], bool, bool] | None:
    name_match = re.search(
        rf"\b{re.escape(declaration.name)}\s*\(", declaration.text
    )
    if name_match is None:
        return None

    return_type = declaration.text[: name_match.start()]
    return_type = re.sub(r"^\s*extern\b", "", return_type)
    return_type = re.sub(r"\s+", " ", return_type).strip()
    return_type = re.sub(r"\s*\*\s*", "*", return_type)

    open_paren = declaration.text.find("(", name_match.start())
    depth = 0
    close_paren = None
    for index in range(open_paren, len(declaration.text)):
        char = declaration.text[index]
        if char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
            if depth == 0:
                close_paren = index
                break
    if close_paren is None:
        return None

    parameters = declaration.text[open_paren + 1 : close_paren].strip()
    if not parameters:
        return return_type, (), False, True
    if parameters == "void":
        return return_type, (), False, False

    parts = []
    start = 0
    depth = 0
    for index, char in enumerate(parameters):
        if char in "([":
            depth += 1
        elif char in ")]":
            depth -= 1
        elif char == "," and depth == 0:
            parts.append(parameters[start:index].strip())
            start = index + 1
    parts.append(parameters[start:].strip())

    variadic = parts[-1] == "..."
    if variadic:
        parts.pop()
    return (
        return_type,
        tuple(normalize_parameter(part) for part in parts),
        variadic,
        False,
    )


def normalize_parameter(parameter: str) -> str:
    parameter = re.sub(
        r"\(\s*\*\s*[A-Za-z_][A-Za-z0-9_]*\s*\)",
        "(*)",
        parameter,
    )
    parameter = re.sub(
        r"\b[A-Za-z_][A-Za-z0-9_]*\s*(?=\[)",
        "",
        parameter,
    )

    identifiers = list(re.finditer(r"\b[A-Za-z_][A-Za-z0-9_]*\b", parameter))
    type_keywords = {
        "char",
        "const",
        "double",
        "enum",
        "float",
        "int",
        "long",
        "register",
        "restrict",
        "short",
        "signed",
        "struct",
        "union",
        "unsigned",
        "volatile",
        "void",
    }
    if identifiers:
        last = identifiers[-1]
        suffix = parameter[last.end() :].strip()
        all_keywords = all(
            match.group() in type_keywords for match in identifiers
        )
        if (
            suffix == ""
            and len(identifiers) > 1
            and not all_keywords
            and not (
                len(identifiers) == 2
                and identifiers[0].group() in {"struct", "union", "enum"}
            )
        ):
            parameter = parameter[: last.start()]

    parameter = re.sub(r"\s+", " ", parameter).strip()
    parameter = re.sub(r"\s*\*\s*", "*", parameter)
    parameter = re.sub(r"\s*([\[\],()])\s*", r"\1", parameter)
    return parameter


def signatures_are_compatible(source: Declaration, header: Declaration) -> bool:
    source_signature = declaration_signature(source)
    header_signature = declaration_signature(header)
    if source_signature is None or header_signature is None:
        return False

    source_return, source_parameters, source_variadic, source_unspecified = (
        source_signature
    )
    header_return, header_parameters, header_variadic, header_unspecified = (
        header_signature
    )
    if source_return != header_return:
        return False
    return (
        len(source_parameters) == len(header_parameters)
        and all(
            parameters_are_compatible(source_parameter, header_parameter)
            for source_parameter, header_parameter in zip(
                source_parameters, header_parameters
            )
        )
        and source_variadic == header_variadic
        and source_unspecified == header_unspecified
    )


def parameters_are_compatible(source: str, header: str) -> bool:
    if source == header:
        return True

    scalar_type = re.compile(
        r"^(?:(?:const|volatile) )?"
        r"(?:char|short|int|long|float|double|"
        r"[us](?:8|16|32|64)|f(?:32|64)|OSPri|OSId)$"
    )
    if source == "void*" and (
        "*" in header or scalar_type.fullmatch(header) is None
    ):
        return True
    if header == "void*" and (
        "*" in source or scalar_type.fullmatch(source) is None
    ):
        return True
    return False


def function_definitions(text: str) -> dict[str, int]:
    masked = mask_comments_and_literals(text)
    return {
        match.group("name"): match.start()
        for match in FUNCTION_DEFINITION.finditer(masked)
    }


def declaration_section_end(text: str, first_definition: int) -> int:
    """Keep moved declarations outside conditional implementation blocks."""
    conditional_stack: list[int] = []
    offset = 0
    for line in text[:first_definition].splitlines(keepends=True):
        directive = line.lstrip()
        if re.match(r"#\s*(?:if|ifdef|ifndef)\b", directive):
            conditional_stack.append(offset)
        elif re.match(r"#\s*endif\b", directive) and conditional_stack:
            conditional_stack.pop()
        offset += len(line)
    return conditional_stack[0] if conditional_stack else first_definition


def header_index(
    include_dir: Path,
    prefixes: tuple[Path, ...] | None = None,
    exclude_internal: bool = False,
) -> dict[str, tuple[Declaration, ...]]:
    declarations: dict[str, list[Declaration]] = defaultdict(list)
    for header in sorted(include_dir.rglob("*.h")):
        relative_header = header.relative_to(include_dir)
        if prefixes is not None and not any(
            relative_header.is_relative_to(prefix) for prefix in prefixes
        ):
            continue
        if exclude_internal and "internal" in relative_header.stem.lower():
            continue
        for declaration in find_declarations(header, source_externs=False):
            declarations[declaration.name].append(
                Declaration(
                    path=relative_header,
                    start=declaration.start,
                    end=declaration.end,
                    line=declaration.line,
                    name=declaration.name,
                    text=declaration.text,
                )
            )
    return {
        name: tuple(sorted(items, key=lambda item: (item.path, item.line)))
        for name, items in declarations.items()
    }


def source_files(paths: Sequence[Path]) -> Iterable[Path]:
    seen: set[Path] = set()
    for path in paths:
        files = [path] if path.is_file() else sorted(path.rglob("*.c"))
        for source in files:
            resolved = source.resolve()
            ignored = subprocess.run(
                ["git", "check-ignore", "-q", str(source)],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                check=False,
            ).returncode == 0
            if source.suffix == ".c" and resolved not in seen and not ignored:
                seen.add(resolved)
                yield source


def expected_header_for_source(source: Path, repo_root: Path) -> Path | None:
    try:
        relative = source.relative_to(repo_root / "src").with_suffix(".h")
    except ValueError:
        return None
    return Path("game") / relative


def header_score(
    header: Path,
    included: set[str],
    expected_header: Path | None,
) -> tuple[int, int, int]:
    return (
        0 if header.as_posix() in included else 1,
        0 if header == expected_header else 1,
        1 if "internal" in header.stem.lower() else 0,
    )


def select_header(
    candidates: tuple[Path, ...],
    included: set[str],
    expected_header: Path | None,
) -> Path | None:
    if len(candidates) == 1:
        return candidates[0]

    ranked = sorted(
        (header_score(header, included, expected_header), header)
        for header in candidates
    )
    if ranked[0][0] == ranked[1][0]:
        return None
    return ranked[0][1]


def find_violations(
    sources: Iterable[Path],
    headers: dict[str, tuple[Declaration, ...]],
    repo_root: Path,
) -> list[Violation]:
    violations = []
    for source in sources:
        text = source.read_text()
        included = set(INCLUDE.findall(text))
        expected_header = expected_header_for_source(source, repo_root)
        source_declarations = find_declarations(source, source_externs=True)
        definitions = function_definitions(text)
        first_definition = min(definitions.values(), default=len(text))
        unsafe_headers = set()
        for source_declaration in source_declarations:
            declarations_by_header: dict[Path, list[Declaration]] = defaultdict(
                list
            )
            for header in headers.get(source_declaration.name, ()):
                declarations_by_header[header.path].append(header)
            unsafe_headers.update(
                header_path
                for header_path, header_declarations in declarations_by_header.items()
                if not any(
                    signatures_are_compatible(source_declaration, header)
                    for header in header_declarations
                )
            )

        for declaration in source_declarations:
            if (
                declaration.name in definitions
                and declaration.start > first_definition
            ):
                violations.append(
                    Violation(
                        declaration=declaration,
                        candidates=(),
                        selected_header=None,
                        same_file_forward=True,
                    )
                )
                continue

            candidate_declarations = headers.get(declaration.name, ())
            candidates = tuple(
                sorted(
                    {
                        header.path
                        for header in candidate_declarations
                        if signatures_are_compatible(declaration, header)
                        and header.path not in unsafe_headers
                    }
                )
            )
            if not candidates:
                continue
            violations.append(
                Violation(
                    declaration=declaration,
                    candidates=candidates,
                    selected_header=select_header(
                        candidates, included, expected_header
                    ),
                )
            )
    return violations


def include_insertion_offset(text: str) -> int:
    matches = list(INCLUDE.finditer(text))
    if matches:
        line_end = text.find("\n", matches[-1].end())
        return len(text) if line_end == -1 else line_end + 1

    leading_comment = re.match(
        r"\A(?:[ \t]*\n|[ \t]*/\*.*?\*/[ \t]*\n|[ \t]*//[^\n]*\n)*",
        text,
        re.DOTALL,
    )
    return leading_comment.end() if leading_comment else 0


def fix_file(path: Path, violations: Sequence[Violation]) -> None:
    text = path.read_text()
    selected = [
        item
        for item in violations
        if item.selected_header is not None or item.same_file_forward
    ]
    if not selected:
        return

    included = set(INCLUDE.findall(text))
    headers_to_add = sorted(
        {
            header
            for item in selected
            if item.selected_header is not None
            for header in (
                *HEADER_PREREQUISITES.get(
                    item.selected_header.as_posix(), ()
                ),
                item.selected_header.as_posix(),
            )
            if header not in included
        }
    )

    for item in sorted(
        selected, key=lambda violation: violation.declaration.start, reverse=True
    ):
        declaration = item.declaration
        text = text[: declaration.start] + text[declaration.end :]
        if (
            item.same_file_forward
            and text[max(0, declaration.start - 2) : declaration.start + 1]
            == "\n\n\n"
        ):
            text = text[: declaration.start] + text[declaration.start + 1 :]

    forwards_to_move = [
        item.declaration.text.strip()
        for item in selected
        if item.same_file_forward
    ]
    if forwards_to_move:
        definitions = function_definitions(text)
        first_definition = min(definitions.values())
        insertion_offset = declaration_section_end(text, first_definition)
        forward_text = "".join(
            f"{declaration}\n" for declaration in forwards_to_move
        )
        text = (
            text[:insertion_offset]
            + forward_text
            + "\n"
            + text[insertion_offset:]
        )

    if headers_to_add:
        offset = include_insertion_offset(text)
        include_text = "".join(
            f'#include "{header}"\n' for header in headers_to_add
        )
        text = text[:offset] + include_text + text[offset:]

    path.write_text(text)


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "paths",
        nargs="*",
        type=Path,
        default=[Path("src")],
        help="C files or directories to check (default: src)",
    )
    parser.add_argument(
        "--include-dir",
        type=Path,
        default=Path("include"),
        help="header tree to index (default: include)",
    )
    parser.add_argument(
        "--fix",
        action="store_true",
        help="remove redundant externs and add unambiguous header includes",
    )
    parser.add_argument(
        "--all-headers",
        action="store_true",
        help="also audit vendored headers outside include/game",
    )
    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    repo_root = Path.cwd().resolve()
    include_dir = args.include_dir.resolve()

    missing = [path for path in [args.include_dir, *args.paths] if not path.exists()]
    if missing:
        for path in missing:
            print(f"error: path does not exist: {path}", file=sys.stderr)
        return 2

    violations = find_violations(
        source_files(args.paths),
        header_index(
            include_dir,
            prefixes=None if args.all_headers else (Path("game"),),
            exclude_internal=not args.all_headers,
        ),
        repo_root,
    )

    by_file: dict[Path, list[Violation]] = defaultdict(list)
    for violation in violations:
        by_file[violation.declaration.path].append(violation)
        location = (
            f"{violation.declaration.path}:{violation.declaration.line}: "
            f"redundant extern for {violation.declaration.name}"
        )
        if violation.selected_header is not None:
            print(f"{location}; declared by {violation.selected_header}")
        elif violation.same_file_forward:
            print(f"{location}; move before the first function definition")
        else:
            choices = ", ".join(str(path) for path in violation.candidates)
            print(f"{location}; ambiguous headers: {choices}")

    if args.fix:
        for path, file_violations in by_file.items():
            fix_file(path, file_violations)

        fixed = sum(
            violation.selected_header is not None
            or violation.same_file_forward
            for violation in violations
        )
        ambiguous = len(violations) - fixed
        print(f"fixed {fixed} redundant extern(s); {ambiguous} ambiguous")
        return 1 if ambiguous else 0

    if violations:
        print(f"found {len(violations)} redundant extern(s)", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
