#!/usr/bin/env bash
set -euo pipefail

# stdin: JSON with .tool_input.command (Claude Bash) or .tool_input.cmd (Codex exec_command)
input=$(cat)

command=$(echo "$input" | jq -r '.tool_input.command // .tool_input.cmd // ""')

if [[ -z "$command" ]]; then
  exit 0
fi

COMMAND="$command" python3 - <<'PY'
import math
import os
import re
import shlex
import sys
import time


RESERVED_SECONDS = 120
BOUNDARIES = {";", "&&", "||", "|"}


def is_assignment(token: str) -> bool:
    return re.match(r"^[A-Za-z_][A-Za-z0-9_]*=", token) is not None


def is_permuter_token(token: str) -> bool:
    normalized = token.lstrip("./")
    return normalized in {
        "tools/permuter",
        "tools/decomp-permuter/permuter.py",
    }


def find_permuter_index(tokens: list[str]) -> int | None:
    for index, token in enumerate(tokens):
        if is_permuter_token(token):
            return index
    return None


def find_segment_start(tokens: list[str], index: int) -> int:
    segment_start = 0
    for token_index in range(index - 1, -1, -1):
        if tokens[token_index] in BOUNDARIES:
            segment_start = token_index + 1
            break
    return segment_start


def parse_duration_seconds(duration: str) -> int | None:
    match = re.fullmatch(r"([0-9]+(?:\.[0-9]+)?)([smhd]?)", duration)
    if match is None:
        return None

    value = float(match.group(1))
    suffix = match.group(2)
    multiplier = {
        "": 1,
        "s": 1,
        "m": 60,
        "h": 60 * 60,
        "d": 24 * 60 * 60,
    }[suffix]
    seconds = math.floor(value * multiplier)
    return seconds if seconds > 0 else None


def timeout_duration(tokens: list[str], segment_start: int, permuter_index: int) -> int | None:
    index = segment_start
    while index < permuter_index and is_assignment(tokens[index]):
        index += 1

    if index >= permuter_index or tokens[index] != "timeout":
        return None

    index += 1
    while index < permuter_index:
        token = tokens[index]
        if token in {"--foreground", "--preserve-status", "-v"}:
            index += 1
            continue
        if token in {"-k", "--kill-after", "-s", "--signal"}:
            index += 2
            continue
        if token.startswith("--kill-after=") or token.startswith("--signal="):
            index += 1
            continue
        if token.startswith("-") and token != "-":
            index += 1
            continue
        return parse_duration_seconds(token)

    return None


command = os.environ["COMMAND"]
try:
    tokens = shlex.split(command)
except ValueError as exc:
    print(f"Blocked: unable to parse command while checking permuter timeout: {exc}", file=sys.stderr)
    sys.exit(2)

permuter_index = find_permuter_index(tokens)
if permuter_index is None:
    sys.exit(0)

segment_start = find_segment_start(tokens, permuter_index)
seconds = timeout_duration(tokens, segment_start, permuter_index)
if seconds is None:
    print(
        "Blocked: permuter runs must be wrapped with a valid shell timeout, e.g. "
        "timeout 300s ./tools/permuter --source-file ...",
        file=sys.stderr,
    )
    sys.exit(2)

deadline = os.environ.get("NIGEL_TIMEOUT_DEADLINE_UNIX", "")
if not deadline:
    sys.exit(0)

try:
    deadline_unix = int(deadline)
except ValueError:
    print("Blocked: NIGEL_TIMEOUT_DEADLINE_UNIX is set but is not a valid Unix timestamp.", file=sys.stderr)
    sys.exit(2)

latest_finish = deadline_unix - RESERVED_SECONDS
finish_at = int(time.time()) + seconds
if finish_at > latest_finish:
    allowed = latest_finish - int(time.time())
    limit = f"Use timeout no greater than {allowed}s." if allowed > 0 else "Skip the run."
    print(
        "Blocked: permuter timeout would leave less than 120 seconds before "
        f"NIGEL_TIMEOUT_DEADLINE_UNIX. {limit}",
        file=sys.stderr,
    )
    sys.exit(2)
PY

exit 0
