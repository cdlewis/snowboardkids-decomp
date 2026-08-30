#!/usr/bin/env python3
"""Reject hardcoded references to known base-ROM segment boundaries."""

import argparse
import re
import subprocess
from pathlib import Path


HEX_RE = re.compile(r"\b0x0*([0-9A-Fa-f]{4,8})\b")
LEGACY_RE = re.compile(r"\bD_([0-7][0-9A-Fa-f]{5,7})\b")
YAML_ADDRESS_RE = re.compile(r"(?:start|rom_start|rom_end):\s*['\"]?0x([0-9A-Fa-f]+)")
YAML_LIST_RE = re.compile(r"-\s*\[\s*0x([0-9A-Fa-f]+)")


def strip_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", lambda match: "\n" * match.group(0).count("\n"), text, flags=re.DOTALL)
    return re.sub(r"//.*", "", text)


def load_boundaries(root: Path) -> set[int]:
    paths = [root / "snowboardkids.yaml", *(root / "config" / "courses").glob("*.yaml")]
    boundaries = set()
    for path in paths:
        text = path.read_text()
        boundaries.update(int(value, 16) for value in YAML_ADDRESS_RE.findall(text))
        boundaries.update(int(value, 16) for value in YAML_LIST_RE.findall(text))
    # The highest marker is the exclusive end/declared capacity of the ROM, not
    # a readable runtime address. It is also a common fixed-point/flag value.
    boundaries.discard(max(boundaries))
    return boundaries


def load_allowlist(path: Path) -> set[tuple[str, str]]:
    entries = set()
    for number, line in enumerate(path.read_text().splitlines(), 1):
        stripped = line.strip()
        if not stripped or stripped.startswith("#"):
            continue
        parts = stripped.split(maxsplit=2)
        if len(parts) < 3:
            raise SystemExit(f"{path}:{number}: expected: path token reason")
        entries.add((parts[0], parts[1]))
    return entries


def source_files(root: Path):
    result = subprocess.run(
        ["git", "ls-files", "src", "include", "asm", "linker_scripts"],
        cwd=root,
        check=True,
        capture_output=True,
        text=True,
    )
    for relative in result.stdout.splitlines():
        path = root / relative
        if path.suffix in {".c", ".h", ".s", ".ld"}:
            yield path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--allowlist", type=Path)
    args = parser.parse_args()
    root = args.root.resolve()
    allowlist_path = args.allowlist or root / "config" / "rom_address_allowlist.txt"
    boundaries = load_boundaries(root)
    allowlist = load_allowlist(allowlist_path)
    findings = []

    for path in source_files(root):
        relative = path.relative_to(root).as_posix()
        original_lines = path.read_text(errors="replace").splitlines()
        clean_lines = strip_comments("\n".join(original_lines)).splitlines()
        for line_number, (original, line) in enumerate(zip(original_lines, clean_lines), 1):
            for match in HEX_RE.finditer(line):
                token = match.group(0)
                value = int(match.group(1), 16)
                if value >= 0x10000 and value in boundaries and (relative, token) not in allowlist:
                    findings.append((relative, line_number, token, original.strip()))
            for match in LEGACY_RE.finditer(line):
                token = match.group(0)
                value = int(match.group(1), 16)
                safe_alias = re.search(rf"\b{re.escape(token)}\s*=.*_ROM_(?:START|END)\b", line)
                if value in boundaries and not safe_alias and (relative, token) not in allowlist:
                    findings.append((relative, line_number, token, original.strip()))

    if findings:
        for relative, line_number, token, line in findings:
            print(f"{relative}:{line_number}: hardcoded ROM boundary {token}: {line}")
        raise SystemExit(f"found {len(findings)} hardcoded ROM boundary reference(s)")
    print(f"ROM address scan OK: {len(boundaries)} known boundaries, no forbidden references")


if __name__ == "__main__":
    main()
