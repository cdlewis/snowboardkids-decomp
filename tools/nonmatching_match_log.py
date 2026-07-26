#!/usr/bin/env python3
"""Parse and validate progress entries from nonmatching workspaces."""

from __future__ import annotations

import hashlib
import re
from dataclasses import dataclass
from pathlib import Path


MATCH_LINE_RE = re.compile(
    r"(?P<filename>[A-Za-z0-9_./-]+\.c)\s+"
    r"(?P<percent>\d+(?:\.\d+)?)\s*%"
    r"(?P<metadata>.*)$"
)
SHA256_RE = re.compile(r"(?:^|\s)sha256=(?P<sha256>[0-9a-f]{64})(?:\s|$)")


@dataclass(frozen=True)
class MatchLogEntry:
    filename: str
    percent: float
    exact: bool
    sha256: str | None


def parse_match_log_line(line: str) -> MatchLogEntry | None:
    match = MATCH_LINE_RE.search(line)
    if match is None:
        return None

    try:
        percent = float(match.group("percent"))
    except ValueError:
        return None

    metadata = match.group("metadata").split()
    sha_match = SHA256_RE.search(match.group("metadata"))
    return MatchLogEntry(
        filename=match.group("filename"),
        percent=percent,
        exact="exact" in metadata,
        sha256=sha_match.group("sha256") if sha_match is not None else None,
    )


def file_sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def certified_attempt_path(entry: MatchLogEntry, workspace: Path) -> Path | None:
    """Resolve an entry, rejecting uncertified or modified full matches."""
    attempt = (workspace / entry.filename).resolve()

    if entry.percent < 100.0:
        return attempt

    if not entry.exact or entry.sha256 is None:
        return None

    match_dir = (workspace / ".matches").resolve()
    if match_dir not in attempt.parents or not attempt.is_file():
        return None
    if not attempt.name.endswith(f"-{entry.sha256}.c"):
        return None

    try:
        if file_sha256(attempt) != entry.sha256:
            return None
    except OSError:
        return None
    return attempt
