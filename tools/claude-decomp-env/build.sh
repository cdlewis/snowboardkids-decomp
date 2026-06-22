#!/usr/bin/env bash
set -euo pipefail

if [ $# -lt 1 ]; then
    echo "Usage: $0 <file.c> [opt_flag]"
    exit 1
fi

INPUT="$(realpath "$1")"
OPT_FLAG="${2:--O1}"
OBJECT_OUTPUT="$(realpath "${1%.c}.o")"
ANNOTATED_OUTPUT="$(realpath "${1%.c}_annotated.s")"
OBJECT_DUMP="${1%.c}_object_dump.s"

if grep -q "INCLUDE_ASM\|GLOBAL_ASM" "$INPUT"; then
    echo "ERROR: The C file contains an assembly include."
    echo "Write C code that compiles to matching assembly instead."
    exit 1
fi

SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_PATH/../.." && pwd)"

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
CFLAGS=(-c "$OPT_FLAG" -mips2 -G 0 -non_shared -fullwarn -Xcpluscomm \
    -nostdinc -Wab,-r4300_mul -woff 649,838,712,516 \
    -I"$PROJECT_ROOT/include" "${C_DEFINES[@]}")

pushd "$PROJECT_ROOT" >/dev/null

python3 "$ASM_PROC" "$CC" -- "$AS" "${ASFLAGS[@]}" -- "${CFLAGS[@]}" -o "$OBJECT_OUTPUT" "$INPUT"
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

SCORE_OUTPUT=$(python3 dist.py target.o "$OBJECT_OUTPUT" --stack-diffs)
echo "$SCORE_OUTPUT"

MATCH_PERCENT=$(echo "$SCORE_OUTPUT" | grep -oP 'Score: \K[0-9.]+' || true)
if [[ $1 =~ base_[0-9]+ ]] && [ -n "$MATCH_PERCENT" ]; then
    echo "$1 ${MATCH_PERCENT}%" >> match_log.txt

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
        if (since > 10) {
            printf "%d %s %.1f\n", since, best_file, best
        }
    }' match_log.txt)

    if [ -n "$STALL_INFO" ]; then
        read -r SINCE BEST_FILE BEST_SCORE <<< "$STALL_INFO"
        echo "No progress in $SINCE attempts (best: ${BEST_SCORE}% at $BEST_FILE). STOP and report your findings."
    fi
fi
