#!/usr/bin/env bash
set -euo pipefail

if [ $# -lt 1 ]; then
    echo "Usage: $0 <file.c> [opt_flag]"
    exit 1
fi

INPUT="$(realpath "$1")"
OPT_FLAG="${2:--O2}"
OBJECT_OUTPUT="$(realpath "${1%.c}.o")"
ANNOTATED_OUTPUT="$(realpath "${1%.c}_annotated.s")"
OBJECT_DUMP="${1%.c}_object_dump.s"
WORKSPACE="$(pwd -P)"
DIFF_ALGORITHM=sequence

if [ -f "$WORKSPACE/.diff_algorithm" ]; then
    DIFF_ALGORITHM="$(tr -d '[:space:]' < "$WORKSPACE/.diff_algorithm")"
fi
if [ "$DIFF_ALGORITHM" != sequence ] && [ "$DIFF_ALGORITHM" != levenshtein ]; then
    echo "ERROR: Unsupported diff algorithm in $WORKSPACE/.diff_algorithm: $DIFF_ALGORITHM"
    exit 1
fi

if ! command -v flock >/dev/null 2>&1; then
    echo "ERROR: flock is required to serialize matching builds."
    exit 1
fi

exec 9>"$WORKSPACE/.build.lock"
flock 9

if grep -q "INCLUDE_ASM\|GLOBAL_ASM" "$INPUT"; then
    echo "ERROR: The C file contains an assembly include."
    echo "Write C code that compiles to matching assembly instead."
    exit 1
fi

# Agents: This restriction is intentional; do not remove, disable, or bypass it.
if python3 - "$INPUT" <<'PY'
import re
import sys

with open(sys.argv[1], "r", encoding="utf-8", errors="surrogateescape") as source_file:
    source = source_file.read()

comments_and_literals = re.compile(
    r'//[^\r\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
    re.DOTALL,
)
source = comments_and_literals.sub(" ", source)
sys.exit(0 if re.search(r"\bdo\b", source) else 1)
PY
then
    echo "ERROR: The C file contains a do-while loop."
    echo "Rewrite the loop using while or for instead."
    exit 1
fi

SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_PATH/../.." && pwd)"
TEXTCONV="$PROJECT_ROOT/tools/textconv.py"
CHARMAP="$PROJECT_ROOT/tools/charmap.txt"

SOURCE_SNAPSHOT="$(mktemp "$WORKSPACE/.build-source.XXXXXX.c")"
cleanup() {
    rm -f -- "$SOURCE_SNAPSHOT"
    if [ -n "${MATCH_BACKUP_TMP:-}" ]; then
        rm -f -- "$MATCH_BACKUP_TMP"
    fi
}
trap cleanup EXIT
python3 "$TEXTCONV" "$CHARMAP" "$INPUT" "$SOURCE_SNAPSHOT"

CROSS="mips-linux-gnu-"
if ! command -v "${CROSS}as" >/dev/null 2>&1; then
    CROSS="mips64-linux-gnu-"
fi
if ! command -v "${CROSS}as" >/dev/null 2>&1; then
    CROSS="mips64-elf-"
fi

AS="${CROSS}as"
OBJDUMP="${CROSS}objdump"
OBJCOPY="${CROSS}objcopy"
NM="${CROSS}nm"
CC="$PROJECT_ROOT/tools/ido-recomp/linux/cc"
ASM_PROC="$PROJECT_ROOT/tools/asm-processor/build.py"

ASFLAGS=(-G 0 -I "$PROJECT_ROOT/include" -mips3 -mabi=32)
C_DEFINES=(-DLANGUAGE_C -D_LANGUAGE_C -D_MIPS_SZLONG=32 -DNDEBUG)
CFLAGS=(-c "$OPT_FLAG" -mips1 -G 0 -non_shared -fullwarn -Xcpluscomm \
    -nostdinc -Wab,-r4300_mul -woff 649,838,712,516 \
    -I"$PROJECT_ROOT/include" "${C_DEFINES[@]}")

pushd "$PROJECT_ROOT" >/dev/null

python3 "$ASM_PROC" "$CC" -- "$AS" "${ASFLAGS[@]}" -- "${CFLAGS[@]}" -o "$OBJECT_OUTPUT" "$SOURCE_SNAPSHOT"
"$OBJCOPY" --remove-section .mdebug "$OBJECT_OUTPUT"

{
    "$OBJDUMP" -drz --line-numbers --source "$OBJECT_OUTPUT" > "$ANNOTATED_OUTPUT"
} 2>/dev/null || true

popd >/dev/null

if ! "$NM" "$OBJECT_OUTPUT" 2>/dev/null | grep -q ' T '; then
    echo "ERROR: Compiled object has no text symbols. Check for type conflicts or include issues."
    "$NM" "$OBJECT_OUTPUT" 2>/dev/null || true
    exit 1
fi

python3 ./objdump.py target.o > target_object_dump.s
python3 ./objdump.py "$OBJECT_OUTPUT" > "$OBJECT_DUMP"
echo "Raw decompiled assembly of $1: $OBJECT_DUMP"
echo "Decompiled assembly of $1 with C annotations: $ANNOTATED_OUTPUT"

python3 ./normalize_asm.py target_object_dump.s > target_object_dump_normalized.s
python3 ./normalize_asm.py "$OBJECT_DUMP" > "${1%.c}_object_dump_normalized.s"
diff -u --suppress-common-lines target_object_dump_normalized.s "${1%.c}_object_dump_normalized.s" > "${1%.c}_diff" || true
echo "Comparison with target file: ${1%.c}_diff"

SCORE_OUTPUT=$(python3 dist.py target.o "$OBJECT_OUTPUT" --stack-diffs --algorithm "$DIFF_ALGORITHM")
echo "$SCORE_OUTPUT"

MATCH_PERCENT=$(echo "$SCORE_OUTPUT" | grep -oP 'Score: \K[0-9.]+' || true)
DIFFERENCE_COUNT=$(echo "$SCORE_OUTPUT" | grep -oP '\(\K[0-9]+(?= differences\))' || true)
SCORER_EXACT=$(echo "$SCORE_OUTPUT" | grep -oP '^Exact match: \K(?:yes|no)$' || true)
NORMALIZED_EXACT=no
if cmp -s target_object_dump_normalized.s "${1%.c}_object_dump_normalized.s"; then
    NORMALIZED_EXACT=yes
fi

TRUE_MATCH=no
if [ "$SCORER_EXACT" = yes ] && [ "$DIFFERENCE_COUNT" = 0 ] && [ "$NORMALIZED_EXACT" = yes ]; then
    TRUE_MATCH=yes
fi

echo "Verified exact match: $TRUE_MATCH"

if [ "$TRUE_MATCH" = yes ]; then
    SOURCE_HASH=$(sha256sum "$SOURCE_SNAPSHOT" | awk '{print $1}')
    SOURCE_STEM="$(basename "${INPUT%.c}")"
    MATCH_DIR="$WORKSPACE/.matches"
    MATCH_BACKUP="$MATCH_DIR/${SOURCE_STEM}-${SOURCE_HASH}.c"
    MATCH_BACKUP_REL=".matches/${SOURCE_STEM}-${SOURCE_HASH}.c"

    mkdir -p "$MATCH_DIR"
    if [ -e "$MATCH_BACKUP" ]; then
        if ! cmp -s "$SOURCE_SNAPSHOT" "$MATCH_BACKUP"; then
            echo "ERROR: Exact-match archive hash collision at $MATCH_BACKUP"
            exit 1
        fi
    else
        MATCH_BACKUP_TMP="$(mktemp "$MATCH_DIR/.archive.XXXXXX")"
        cp -- "$SOURCE_SNAPSHOT" "$MATCH_BACKUP_TMP"
        mv -- "$MATCH_BACKUP_TMP" "$MATCH_BACKUP"
        MATCH_BACKUP_TMP=""
    fi

    echo "Exact-match source archived: $MATCH_BACKUP_REL"
fi

if [[ $1 =~ base_[0-9]+ ]] && [ -n "$MATCH_PERCENT" ]; then
    if [ "$TRUE_MATCH" = yes ]; then
        echo "$MATCH_BACKUP_REL 100.000% exact sha256=$SOURCE_HASH source=$(basename "$INPUT")" >> match_log.txt
    else
        echo "$1 ${MATCH_PERCENT}%" >> match_log.txt
    fi

    STALL_INFO=$(awk '
    {
        gsub(/%/, "", $2)
        total++
        if ($2 + 0 > best + 0) {
            best = $2 + 0
            best_file = $1
            best_at = total
        }
    }
    END {
        since = total - best_at
        if (since >= 40) {
            printf "%d %s %.1f\n", since, best_file, best
        }
    }' match_log.txt)

    if [ -n "$STALL_INFO" ]; then
        read -r SINCE BEST_FILE BEST_SCORE <<< "$STALL_INFO"
        echo "No progress in $SINCE attempts (best: ${BEST_SCORE}% at $BEST_FILE). Consider stopping if you have run of out improvement ideas.."
    fi
fi
