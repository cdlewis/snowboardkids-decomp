#!/usr/bin/env python3
"""Generate Codex hook config from Claude hook settings.

This repo keeps Claude and Codex agent configuration in one real directory:
`.claude`. The repository root `.codex` path is a symlink to `.claude`, so the
generated `.claude/hooks.json` is also visible to Codex as `.codex/hooks.json`.
"""

from __future__ import annotations

import argparse
import json
import re
from copy import deepcopy
from pathlib import Path


DEFAULT_CLAUDE_SETTINGS = Path(".claude/settings.json")
DEFAULT_CODEX_HOOKS = Path(".claude/hooks.json")

TOOL_ALIASES = {
    "Bash": ["Bash", "exec_command"],
    "Edit": ["Edit", "apply_patch"],
    "Write": ["Write"],
    "Read": ["Read"],
}


def repo_relative_command(command: str) -> str:
    """Run repo-local hook scripts from the repository root.

    Claude settings already use `.claude/...` paths. Keep those paths canonical
    instead of emitting `.codex/...`; Codex reaches the same files through the
    `.codex -> .claude` symlink.
    """
    if command.startswith(".codex/"):
        return ".claude/" + command[len(".codex/") :]
    return command


def translate_matcher(matcher: str) -> str:
    """Expand a simple Claude matcher into a Codex-compatible regex.

    Existing regex matchers are preserved. Simple tool names get their Codex
    aliases added where the two agents use different tool names.
    """
    if not matcher:
        return matcher

    if re.search(r"[\\^$.*+?()[\]{}|]", matcher):
        return matcher

    aliases = TOOL_ALIASES.get(matcher, [matcher])
    if len(aliases) == 1:
        return aliases[0]
    return "^(" + "|".join(re.escape(alias) for alias in aliases) + ")$"


def generate(claude_settings_path: Path) -> dict:
    settings = json.loads(claude_settings_path.read_text())
    claude_hooks = settings.get("hooks", {})

    codex_hooks = {"hooks": {}}
    for event_name, entries in claude_hooks.items():
        codex_entries = []
        for entry in entries:
            codex_entry = deepcopy(entry)
            codex_entry["matcher"] = translate_matcher(str(entry.get("matcher", "")))

            for hook in codex_entry.get("hooks", []):
                if hook.get("type") == "command" and isinstance(hook.get("command"), str):
                    hook["command"] = repo_relative_command(hook["command"])

            codex_entries.append(codex_entry)

        codex_hooks["hooks"][event_name] = codex_entries

    return codex_hooks


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Generate .claude/hooks.json for Codex from .claude/settings.json."
    )
    parser.add_argument(
        "--claude-settings",
        type=Path,
        default=DEFAULT_CLAUDE_SETTINGS,
        help="Path to Claude settings.json.",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=DEFAULT_CODEX_HOOKS,
        help="Path to write the Codex hooks.json.",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Exit nonzero if the output file is not up to date.",
    )
    args = parser.parse_args()

    generated = generate(args.claude_settings)
    rendered = json.dumps(generated, indent=2) + "\n"

    if args.check:
        current = args.output.read_text() if args.output.exists() else ""
        if current != rendered:
            print(f"{args.output} is not up to date. Run {Path(__file__).as_posix()}.")
            return 1
        return 0

    args.output.write_text(rendered)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
