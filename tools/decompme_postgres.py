#!/usr/bin/env python3
"""Find similar decomp.me matched scratches from a PostgreSQL dump."""

from __future__ import annotations

import argparse
import csv
import io
import json
import re
import subprocess
import sys
from pathlib import Path
from typing import Iterable


csv.field_size_limit(sys.maxsize)

DEFAULT_COMPILER = "ido5.3"
DEFAULT_FLAGS = ("-O2", "-mips1")
BASE_URL = "https://decomp.me/scratch"
INSTRUCTION_COMMENT_RE = re.compile(
    r"^\s*/\*\s*[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s*\*/\s*(?P<instruction>.*)$"
)
REGISTER_RE = re.compile(r"\$(?:[afvts][0-9]+|zero|at|gp|sp|fp|ra|k[01])\b")
FPR_RE = re.compile(r"\$f(?:t|a|v|s)?[0-9]+\b")
SYMBOL_REF_RE = re.compile(r"%(hi|lo)\(([^)]+)\)")
BRANCH_LABEL_RE = re.compile(r"\.L[.$A-Za-z0-9_]+")
HEX_RE = re.compile(r"(?<![A-Za-z0-9_])-?0x[0-9A-Fa-f]+")
DEC_RE = re.compile(r"(?<![A-Za-z0-9_])-?\d+(?![A-Za-z0-9_])")
MEMORY_RE = re.compile(r"(?P<offset>-?(?:0x[0-9A-Fa-f]+|\d+))\((?P<base>[^)]+)\)")


class AsmFingerprint:
    def __init__(self, asm_text: str):
        self.instructions = normalize_asm(asm_text)
        self.opcodes = [instruction.split(maxsplit=1)[0] for instruction in self.instructions]
        self.trigrams = ngrams(self.instructions, 3)
        self.opcode_trigrams = ngrams(self.opcodes, 4)
        self.memory_shapes = extract_memory_shapes(self.instructions)
        self.branch_shape = [
            opcode for opcode in self.opcodes
            if opcode.startswith("b") or opcode in {"j", "jal", "jr"}
        ]


def sql_literal(value: str) -> str:
    return "'" + value.replace("'", "''") + "'"


def like_literal(value: str) -> str:
    return sql_literal(f"%{value}%")


def build_psql_command(args: argparse.Namespace, sql: str) -> list[str]:
    cmd = ["psql", "-X", "-q"]
    if args.database:
        cmd.extend(["-d", args.database])
    if args.dsn:
        cmd.extend(["-d", args.dsn])
    cmd.extend(["-c", sql])
    return cmd


def run_copy_query(args: argparse.Namespace, query: str) -> list[dict[str, str]]:
    sql = f"COPY ({query}) TO STDOUT WITH CSV HEADER"
    try:
        proc = subprocess.run(
            build_psql_command(args, sql),
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
    except FileNotFoundError:
        print("error: psql was not found in PATH", file=sys.stderr)
        raise SystemExit(127)
    if proc.returncode != 0:
        print(proc.stderr, file=sys.stderr, end="")
        raise SystemExit(proc.returncode)

    return list(csv.DictReader(io.StringIO(proc.stdout)))


def common_where(args: argparse.Namespace) -> list[str]:
    where = []
    if args.preset is not None:
        where.append(f"s.preset_id = {args.preset}")
    if args.compiler:
        where.append(f"s.compiler = {sql_literal(args.compiler)}")
    elif args.compiler_like:
        where.append(f"s.compiler LIKE {sql_literal(args.compiler_like)}")
    for flag in args.compiler_flag:
        where.append(f"s.compiler_flags LIKE {like_literal(flag)}")
    return where


def function_key_expr() -> str:
    return "COALESCE(NULLIF(s.diff_label, ''), regexp_replace(s.name, '-[0-9]+$', ''))"


def match_percent_expr() -> str:
    return "((s.max_score - s.score)::double precision / NULLIF(s.max_score, 0)) * 100"


def best_scratches_query(args: argparse.Namespace) -> str:
    where = common_where(args)
    where.append("s.max_score > 0")
    if args.function:
        where.append(f"{function_key_expr()} = {sql_literal(args.function)}")

    return f"""
        WITH ranked AS (
            SELECT
                {function_key_expr()} AS function_name,
                s.slug,
                s.name,
                s.diff_label,
                s.compiler,
                s.compiler_flags,
                s.score,
                s.max_score,
                {match_percent_expr()} AS match_percent,
                s.creation_time,
                s.last_updated,
                row_number() OVER (
                    PARTITION BY {function_key_expr()}
                    ORDER BY {match_percent_expr()} DESC NULLS LAST, s.last_updated DESC
                ) AS rank
            FROM public.coreapp_scratch AS s
            WHERE {" AND ".join(where)}
        )
        SELECT
            function_name,
            slug,
            name,
            diff_label,
            compiler,
            compiler_flags,
            score,
            max_score,
            round(match_percent::numeric, 3) AS match_percent,
            creation_time,
            last_updated,
            {sql_literal(BASE_URL + "/")} || slug AS url
        FROM ranked
        WHERE rank = 1
        ORDER BY match_percent DESC NULLS LAST, function_name
        LIMIT {args.limit}
    """


def source_search_query(args: argparse.Namespace) -> str:
    where = common_where(args)
    where.append(f"s.source_code ILIKE {like_literal(args.search_source)}")
    return scratch_list_query(where, args.limit)


def asm_search_query(args: argparse.Namespace) -> str:
    where = common_where(args)
    where.append(f"asm.data ILIKE {like_literal(args.search_asm)}")
    return f"""
        SELECT
            {function_key_expr()} AS function_name,
            s.slug,
            s.name,
            s.diff_label,
            s.compiler,
            s.compiler_flags,
            s.score,
            s.max_score,
            round(({match_percent_expr()})::numeric, 3) AS match_percent,
            s.creation_time,
            s.last_updated,
            {sql_literal(BASE_URL + "/")} || s.slug AS url
        FROM public.coreapp_scratch AS s
        JOIN public.coreapp_assembly AS a
            ON a.hash = s.target_assembly_id
        JOIN public.coreapp_asm AS asm
            ON asm.hash = a.source_asm_id
        WHERE {" AND ".join(where)}
        ORDER BY s.last_updated DESC
        LIMIT {args.limit}
    """


def scratch_list_query(where: Iterable[str], limit: int) -> str:
    return f"""
        SELECT
            {function_key_expr()} AS function_name,
            s.slug,
            s.name,
            s.diff_label,
            s.compiler,
            s.compiler_flags,
            s.score,
            s.max_score,
            round(({match_percent_expr()})::numeric, 3) AS match_percent,
            s.creation_time,
            s.last_updated,
            {sql_literal(BASE_URL + "/")} || s.slug AS url
        FROM public.coreapp_scratch AS s
        WHERE {" AND ".join(where)}
        ORDER BY s.last_updated DESC
        LIMIT {limit}
    """


def export_query(args: argparse.Namespace) -> str:
    return f"""
        SELECT
            {function_key_expr()} AS function_name,
            s.slug,
            s.name,
            s.diff_label,
            s.compiler,
            s.compiler_flags,
            s.platform,
            s.score,
            s.max_score,
            round(({match_percent_expr()})::numeric, 3) AS match_percent,
            s.creation_time,
            s.last_updated,
            s.source_code,
            s.context,
            asm.data AS target_asm,
            {sql_literal(BASE_URL + "/")} || s.slug AS url
        FROM public.coreapp_scratch AS s
        JOIN public.coreapp_assembly AS a
            ON a.hash = s.target_assembly_id
        LEFT JOIN public.coreapp_asm AS asm
            ON asm.hash = a.source_asm_id
        WHERE s.slug = {sql_literal(args.slug)}
        LIMIT 1
    """


def matched_asm_query(args: argparse.Namespace) -> str:
    where = common_where(args)
    where.append("s.max_score > 0")
    if args.include_match_overrides:
        where.append("(s.score = 0 OR s.match_override)")
    else:
        where.append("s.score = 0")
    where.append("asm.data IS NOT NULL")

    return f"""
        SELECT
            {function_key_expr()} AS function_name,
            s.slug,
            s.name,
            s.diff_label,
            s.compiler,
            s.compiler_flags,
            s.score AS decomp_score,
            s.max_score,
            round(({match_percent_expr()})::numeric, 3) AS match_percent,
            s.last_updated,
            asm.data AS target_asm,
            {sql_literal(BASE_URL + "/")} || s.slug AS url
        FROM public.coreapp_scratch AS s
        JOIN public.coreapp_assembly AS a
            ON a.hash = s.target_assembly_id
        JOIN public.coreapp_asm AS asm
            ON asm.hash = a.source_asm_id
        WHERE {" AND ".join(where)}
    """


def strip_instruction(line: str) -> str | None:
    line = line.strip()
    if not line:
        return None
    match = INSTRUCTION_COMMENT_RE.match(line)
    if match:
        line = match.group("instruction").strip()
    if not line:
        return None
    if line.startswith((
        ".",
        "glabel ",
        "dlabel ",
        "endlabel ",
        "enddlabel ",
        "nonmatching ",
        "/*",
    )):
        return None
    if line.endswith(":"):
        return None
    line = line.split("#", 1)[0].strip()
    return line or None


def register_class(register: str) -> str:
    name = register[1:]
    if name in {"zero", "at", "gp", "sp", "fp", "ra"}:
        return name.upper()
    if name.startswith("a"):
        return "A"
    if name.startswith("v"):
        return "V"
    if name.startswith("t"):
        return "T"
    if name.startswith("s"):
        return "S"
    if name.startswith("k"):
        return "K"
    return "R"


def canonical_token(value: str, prefix: str, mapping: dict[str, str]) -> str:
    if value not in mapping:
        mapping[value] = f"{prefix}{len(mapping)}"
    return mapping[value]


def normalize_asm(asm_text: str) -> list[str]:
    reg_map: dict[str, str] = {}
    fpr_map: dict[str, str] = {}
    symbol_map: dict[str, str] = {}
    label_map: dict[str, str] = {}
    normalized: list[str] = []

    for raw_line in asm_text.splitlines():
        instruction = strip_instruction(raw_line)
        if instruction is None:
            continue

        parts = instruction.split(maxsplit=1)
        opcode = parts[0]
        operands = parts[1] if len(parts) > 1 else ""

        operands = re.sub(r"\bjal\s+\S+", "jal CALL", f"{opcode} {operands}", count=1)
        if operands.startswith("jal "):
            normalized.append(operands)
            continue

        def symbol_repl(match: re.Match[str]) -> str:
            kind = match.group(1)
            symbol = canonical_token(match.group(2), "SYM", symbol_map)
            return f"%{kind}({symbol})"

        operands = SYMBOL_REF_RE.sub(symbol_repl, operands)
        operands = BRANCH_LABEL_RE.sub(lambda m: canonical_token(m.group(0), "LBL", label_map), operands)
        operands = FPR_RE.sub(lambda m: canonical_token(m.group(0), "F", fpr_map), operands)

        def reg_repl(match: re.Match[str]) -> str:
            register = match.group(0)
            cls = register_class(register)
            if cls in {"ZERO", "AT", "GP", "SP", "FP", "RA"}:
                return cls
            return canonical_token(register, cls, reg_map)

        operands = REGISTER_RE.sub(reg_repl, operands)
        operands = MEMORY_RE.sub(lambda m: f"OFF({m.group('base')})", operands)
        operands = HEX_RE.sub("IMM", operands)
        operands = DEC_RE.sub("IMM", operands)
        operands = re.sub(r"\s+", " ", operands).strip()
        normalized.append(f"{opcode} {operands}".strip())

    return normalized


def extract_memory_shapes(instructions: list[str]) -> set[str]:
    return {
        instruction for instruction in instructions
        if "OFF(" in instruction and instruction.split(maxsplit=1)[0] in {
            "lb", "lbu", "lh", "lhu", "lw", "lwc1", "sb", "sh", "sw", "swc1",
            "ld", "sd", "sdc1", "ldc1",
        }
    }


def ngrams(items: list[str], size: int) -> set[tuple[str, ...]]:
    if len(items) < size:
        return set()
    return {tuple(items[i:i + size]) for i in range(len(items) - size + 1)}


def jaccard(left: set, right: set) -> float:
    if not left and not right:
        return 1.0
    if not left or not right:
        return 0.0
    return len(left & right) / len(left | right)


def sequence_ratio(left: list[str], right: list[str]) -> float:
    if not left and not right:
        return 1.0
    if not left or not right:
        return 0.0

    previous = list(range(len(right) + 1))
    current = [0] * (len(right) + 1)
    for i, left_item in enumerate(left, 1):
        current[0] = i
        for j, right_item in enumerate(right, 1):
            cost = 0 if left_item == right_item else 1
            current[j] = min(
                previous[j] + 1,
                current[j - 1] + 1,
                previous[j - 1] + cost,
            )
        previous, current = current, previous
    return 1.0 - (previous[len(right)] / max(len(left), len(right)))


def similarity(query: AsmFingerprint, candidate: AsmFingerprint) -> dict[str, float]:
    instruction = jaccard(query.trigrams, candidate.trigrams)
    opcode = (
        0.65 * jaccard(query.opcode_trigrams, candidate.opcode_trigrams)
        + 0.35 * sequence_ratio(query.opcodes, candidate.opcodes)
    )
    memory = jaccard(query.memory_shapes, candidate.memory_shapes)
    control = sequence_ratio(query.branch_shape, candidate.branch_shape)
    size = min(len(query.instructions), len(candidate.instructions)) / max(
        len(query.instructions), len(candidate.instructions), 1
    )
    total = (
        0.38 * instruction
        + 0.27 * opcode
        + 0.15 * memory
        + 0.12 * control
        + 0.08 * size
    )
    return {
            "similarity": total,
        "instruction": instruction,
        "opcode": opcode,
        "memory": memory,
        "control": control,
        "size": size,
    }


def find_local_asm(function_or_path: str) -> tuple[str, str, Path]:
    candidate = Path(function_or_path)
    if candidate.is_file():
        return candidate.stem, candidate.read_text(errors="ignore"), candidate

    matches = list(Path("asm/nonmatchings").rglob(f"{function_or_path}.s"))
    if not matches:
        matches = list(Path("asm/nonmatchings").rglob(function_or_path))
    if not matches:
        raise SystemExit(f"error: could not find {function_or_path} under asm/nonmatchings")
    if len(matches) > 1:
        match_list = "\n".join(str(match) for match in matches[:20])
        raise SystemExit(f"error: multiple matches for {function_or_path}:\n{match_list}")
    path = matches[0]
    return path.stem, path.read_text(errors="ignore"), path


def similar_matches(args: argparse.Namespace) -> list[dict[str, str]]:
    query_name, query_asm, query_path = find_local_asm(args.similar_to)
    query = AsmFingerprint(query_asm)
    if not query.instructions:
        raise SystemExit(f"error: no instructions parsed from {query_path}")

    rows = run_copy_query(args, matched_asm_query(args))
    results: list[dict[str, str]] = []
    best_by_function: dict[str, dict[str, str]] = {}
    for row in rows:
        target_asm = row.pop("target_asm", "")
        candidate = AsmFingerprint(target_asm)
        if not candidate.instructions:
            continue
        scores = similarity(query, candidate)
        if scores["similarity"] < args.threshold:
            continue
        result = {
            "query_function": query_name,
            "query_path": str(query_path),
            "query_instruction_count": str(len(query.instructions)),
            "candidate_instruction_count": str(len(candidate.instructions)),
            **row,
            **{key: f"{value:.4f}" for key, value in scores.items()},
        }
        if args.all_scratches:
            results.append(result)
        else:
            function_name = result["function_name"]
            best = best_by_function.get(function_name)
            if (
                best is None
                or float(result["similarity"]) > float(best["similarity"])
                or (
                    result["similarity"] == best["similarity"]
                    and result["last_updated"] > best["last_updated"]
                )
            ):
                best_by_function[function_name] = result

    if not args.all_scratches:
        results = list(best_by_function.values())
    results.sort(key=lambda row: (-float(row["similarity"]), row["function_name"], row["slug"]))
    return results[:args.limit]


def print_table(rows: list[dict[str, str]]) -> None:
    if not rows:
        print("No rows.")
        return

    columns = ["function_name", "similarity", "instruction", "opcode", "memory", "slug", "url"]
    if "similarity" not in rows[0]:
        columns = ["function_name", "match_percent", "compiler", "slug", "name", "url"]
    widths = {
        column: max(len(column), *(len(row.get(column, "")) for row in rows))
        for column in columns
    }
    header = "  ".join(column.ljust(widths[column]) for column in columns)
    print(header)
    print("-" * len(header))
    for row in rows:
        print("  ".join(row.get(column, "").ljust(widths[column]) for column in columns))


def write_export(row: dict[str, str], export_dir: Path) -> None:
    export_dir.mkdir(parents=True, exist_ok=True)

    source_code = row.pop("source_code", "")
    context = row.pop("context", "")
    target_asm = row.pop("target_asm", "")

    (export_dir / "source.c").write_text(source_code)
    (export_dir / "ctx.c").write_text(context)
    (export_dir / "target.s").write_text(target_asm)
    (export_dir / "metadata.json").write_text(json.dumps(row, indent=2) + "\n")

    print(f"Wrote {export_dir}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    conn = parser.add_mutually_exclusive_group(required=True)
    conn.add_argument("--database", "-d", help="PostgreSQL database name for psql -d")
    conn.add_argument("--dsn", help="PostgreSQL DSN/connection string passed to psql")

    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--similar-to", help="Local asm/nonmatchings function name or .s path")
    mode.add_argument("--function", help="Return the best scratch for one function name")
    mode.add_argument("--search-source", help="Search scratch source_code with ILIKE")
    mode.add_argument("--search-asm", help="Search target assembly text with ILIKE")
    mode.add_argument("--slug", help="Export one scratch by slug")

    parser.add_argument("--preset", type=int, default=None)
    parser.add_argument("--compiler", default=DEFAULT_COMPILER)
    parser.add_argument("--compiler-like", default=None)
    parser.add_argument("--compiler-flag", action="append", default=list(DEFAULT_FLAGS))
    parser.add_argument(
        "--include-match-overrides",
        action="store_true",
        help="Include match_override scratches in similarity mode; default is exact score=0 only",
    )
    parser.add_argument(
        "--all-scratches",
        action="store_true",
        help="Do not deduplicate similar results by function name",
    )
    parser.add_argument("--threshold", type=float, default=0.0)
    parser.add_argument("--limit", type=int, default=50)
    parser.add_argument("--json", action="store_true", help="Print rows as JSON")
    parser.add_argument("--export-dir", type=Path, help="Directory for --slug export")
    return parser.parse_args()


def main() -> int:
    args = parse_args()

    if args.limit <= 0:
        print("error: --limit must be greater than zero", file=sys.stderr)
        return 2
    if args.export_dir and not args.slug:
        print("error: --export-dir requires --slug", file=sys.stderr)
        return 2
    if args.slug and not args.export_dir:
        args.export_dir = Path("decompme_exports") / args.slug

    if args.slug:
        rows = run_copy_query(args, export_query(args))
        if not rows:
            print(f"error: no scratch found for slug {args.slug}", file=sys.stderr)
            return 1
        write_export(rows[0], args.export_dir)
        return 0

    if args.similar_to:
        rows = similar_matches(args)
        if args.json:
            print(json.dumps(rows, indent=2))
        else:
            print_table(rows)
        return 0

    if args.search_source:
        rows = run_copy_query(args, source_search_query(args))
    elif args.search_asm:
        rows = run_copy_query(args, asm_search_query(args))
    else:
        rows = run_copy_query(args, best_scratches_query(args))

    if args.json:
        print(json.dumps(rows, indent=2))
    else:
        print_table(rows)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
