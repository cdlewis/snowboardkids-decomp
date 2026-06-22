#!/usr/bin/env python3
"""
Try to decompile unmatched functions with m2c and inline exact matches.

This is intended as a cheap pre-pass before spending LLM time. For each
asm/nonmatchings function, it:

1. Generates C with m2c, using m2ctx context from the destination C file.
2. Compiles the generated function in a temporary per-function workspace.
3. If the compiled object has 0 differences from the target, replaces the
   matching #pragma GLOBAL_ASM line in src/ with the generated C.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import m2ctx


ROOT = Path(__file__).resolve().parents[1]
ASM_ROOT = ROOT / "asm" / "nonmatchings"
SRC_ROOT = ROOT / "src"
HELPER_DIR = ROOT / "tools" / "claude-decomp-env"
MACRO_INC = ROOT / "include" / "macro.inc"
PRELUDE_INC = HELPER_DIR / "prelude.inc"
BUILD_SH = HELPER_DIR / "build.sh"

GENERATED_HEADER = """#include "common.h"

// This is a decompilation attempt by the m2c tool.
// Function/type definitions might be missing or incomplete.
// The code will likely not compile without further modification.

"""


def run(
    cmd: list[str | os.PathLike[str]],
    *,
    cwd: Path = ROOT,
    timeout: int | None = None,
) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [str(part) for part in cmd],
        cwd=cwd,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        timeout=timeout,
    )


def find_cross_prefix() -> str | None:
    for prefix in ("mips-linux-gnu-", "mips64-linux-gnu-", "mips64-elf-"):
        if shutil.which(prefix + "as"):
            return prefix
    return None


def asm_path_for_name(name: str) -> Path | None:
    wanted = name if name.endswith(".s") else name + ".s"
    matches = sorted(ASM_ROOT.rglob(wanted))
    return matches[0] if matches else None


def destination_c_file(asm_path: Path) -> Path:
    rel_dir = asm_path.parent.relative_to(ASM_ROOT)
    return SRC_ROOT / rel_dir.with_suffix(".c")


def pragma_for_asm(asm_path: Path) -> str:
    rel = asm_path.relative_to(ROOT).as_posix()
    return f'#pragma GLOBAL_ASM("{rel}")'


def candidate_asm_paths(args: argparse.Namespace) -> list[Path]:
    if args.functions:
        paths: list[Path] = []
        for function in args.functions:
            path = Path(function)
            if path.suffix == ".s":
                if not path.is_absolute():
                    path = ROOT / path
                if not path.exists():
                    raise SystemExit(f"assembly file does not exist: {function}")
                paths.append(path)
                continue

            asm_path = asm_path_for_name(function)
            if asm_path is None:
                raise SystemExit(f"could not find assembly for function: {function}")
            paths.append(asm_path)
    else:
        paths = sorted(ASM_ROOT.rglob("*.s"))

    if args.limit is not None:
        paths = paths[: args.limit]
    return paths


def write_target_object(tmpdir: Path, asm_path: Path, cross_prefix: str) -> bool:
    target_s = tmpdir / "target.s"
    target_o = tmpdir / "target.o"
    with target_s.open("w", encoding="utf-8") as out:
        for path in (PRELUDE_INC, MACRO_INC, asm_path):
            out.write(path.read_text(encoding="utf-8"))
            out.write("\n")

    result = run(
        [
            cross_prefix + "as",
            "-EB",
            "-march=vr4300",
            "-mtune=vr4300",
            "-Iinclude",
            "-o",
            target_o,
            target_s,
        ]
    )
    return result.returncode == 0


def link_helpers(tmpdir: Path) -> None:
    for helper in ("objdump.py", "normalize_asm.py", "dist.py"):
        link = tmpdir / helper
        if not link.exists():
            link.symlink_to(HELPER_DIR / helper)


def generate_context(c_file: Path, tmpdir: Path) -> Path | None:
    if not c_file.exists():
        return None
    try:
        context = m2ctx.import_c_file(str(c_file))
    except (subprocess.CalledProcessError, SystemExit):
        return None

    context_path = tmpdir / "m2c_context.c"
    context_path.write_text(context, encoding="utf-8")
    return context_path


def generate_m2c(asm_path: Path, c_file: Path, tmpdir: Path, timeout: int) -> str | None:
    cmd = ["m2c", "--target", "mips-ido-c"]
    context_path = generate_context(c_file, tmpdir)
    if context_path is not None:
        cmd.extend(["--context", str(context_path)])
    cmd.append(str(asm_path))

    try:
        result = run(cmd, timeout=timeout)
    except subprocess.TimeoutExpired:
        return None
    if result.returncode != 0:
        return None
    return result.stdout


def write_candidate(tmpdir: Path, m2c_output: str) -> Path:
    candidate = tmpdir / "base.c"
    candidate.write_text(GENERATED_HEADER + m2c_output.rstrip() + "\n", encoding="utf-8")
    return candidate


def build_matches(tmpdir: Path, candidate: Path, timeout: int) -> bool:
    try:
        result = run([BUILD_SH, candidate], cwd=tmpdir, timeout=timeout)
    except subprocess.TimeoutExpired:
        return False
    if result.returncode != 0:
        return False
    return re.search(r"Score:\s+100\.000%\s+\(0 differences\)", result.stdout) is not None


def strip_candidate_for_inline(candidate_text: str) -> str:
    lines = candidate_text.splitlines()
    while lines and (
        not lines[0].strip()
        or lines[0].startswith("#include ")
        or lines[0].startswith("// This is a decompilation attempt")
        or lines[0].startswith("// Function/type definitions")
        or lines[0].startswith("// The code will likely")
    ):
        lines.pop(0)
    while lines and not lines[0].strip():
        lines.pop(0)
    while lines and not lines[-1].strip():
        lines.pop()
    return "\n".join(lines) + "\n"


def inline_match(c_file: Path, asm_path: Path, candidate_text: str, *, dry_run: bool) -> bool:
    pragma = pragma_for_asm(asm_path)
    source = c_file.read_text(encoding="utf-8")
    if pragma not in source:
        return False

    inline_text = strip_candidate_for_inline(candidate_text)
    if "GLOBAL_ASM" in inline_text or "INCLUDE_ASM" in inline_text:
        return False
    if dry_run:
        return True

    updated = source.replace(pragma, inline_text.rstrip(), 1)
    c_file.write_text(updated, encoding="utf-8")
    return True


def try_function(
    asm_path: Path,
    *,
    cross_prefix: str,
    m2c_timeout: int,
    build_timeout: int,
    dry_run: bool,
) -> str:
    c_file = destination_c_file(asm_path)
    pragma = pragma_for_asm(asm_path)
    if not c_file.exists():
        return "skip:no-c-file"
    if pragma not in c_file.read_text(encoding="utf-8"):
        return "skip:no-pragma"

    with tempfile.TemporaryDirectory(prefix="m2c-one-shot-") as tmp:
        tmpdir = Path(tmp)
        link_helpers(tmpdir)
        if not write_target_object(tmpdir, asm_path, cross_prefix):
            return "skip:target-asm"

        m2c_output = generate_m2c(asm_path, c_file, tmpdir, m2c_timeout)
        if not m2c_output:
            return "miss:m2c"

        candidate = write_candidate(tmpdir, m2c_output)
        if not build_matches(tmpdir, candidate, build_timeout):
            return "miss:build"

        if not inline_match(c_file, asm_path, candidate.read_text(encoding="utf-8"), dry_run=dry_run):
            return "skip:inline"
        return "match"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("functions", nargs="*", help="Optional function names or asm paths to try")
    parser.add_argument("--limit", type=int, help="Only try the first N candidates")
    parser.add_argument("--dry-run", action="store_true", help="Report matches without editing source")
    parser.add_argument("--json", action="store_true", help="Print a JSON summary at the end")
    parser.add_argument("--m2c-timeout", type=int, default=30, help="Seconds before giving up on one m2c run")
    parser.add_argument("--build-timeout", type=int, default=60, help="Seconds before giving up on one build")
    args = parser.parse_args()

    if shutil.which("m2c") is None:
        raise SystemExit("m2c is not installed or is not on PATH")

    cross_prefix = find_cross_prefix()
    if cross_prefix is None:
        raise SystemExit("unable to detect a suitable MIPS assembler")

    paths = candidate_asm_paths(args)
    summary: dict[str, list[str]] = {}
    for index, asm_path in enumerate(paths, start=1):
        rel = asm_path.relative_to(ROOT).as_posix()
        status = try_function(
            asm_path,
            cross_prefix=cross_prefix,
            m2c_timeout=args.m2c_timeout,
            build_timeout=args.build_timeout,
            dry_run=args.dry_run,
        )
        summary.setdefault(status, []).append(rel)
        print(f"[{index}/{len(paths)}] {status:15s} {rel}", flush=True)

    if args.json:
        print(json.dumps(summary, indent=2, sort_keys=True))
    else:
        counts = ", ".join(f"{key}={len(value)}" for key, value in sorted(summary.items()))
        print(f"Summary: {counts}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
