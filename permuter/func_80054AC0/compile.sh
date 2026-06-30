#!/usr/bin/env bash
set -euo pipefail
INPUT="$(realpath "$1")"
OBJ="$(realpath "$2")"
ROOT="/home/chris/code/snowboardkids-decomp"
AS="mips-linux-gnu-as"
OBJCOPY="mips-linux-gnu-objcopy"
[ -x "$(command -v $AS)" ] || { AS="mips64-linux-gnu-as"; OBJCOPY="mips64-linux-gnu-objcopy"; }
CC="$ROOT/tools/ido-recomp/linux/cc"
ASM_PROC="$ROOT/tools/asm-processor/build.py"
ASFLAGS=(-G 0 -I "$ROOT/include" -mips3 -mabi=32)
CFLAGS=(-c -O2 -mips1 -G 0 -non_shared -fullwarn -Xcpluscomm -nostdinc -Wab,-r4300_mul -woff 649,838,712,516 -I"$ROOT/include" -DLANGUAGE_C -D_LANGUAGE_C -D_MIPS_SZLONG=32 -DNDEBUG)
cd "$ROOT"
python3 "$ASM_PROC" "$CC" -- "$AS" "${ASFLAGS[@]}" -- "${CFLAGS[@]}" -o "$OBJ" "$INPUT"
"$OBJCOPY" --remove-section .mdebug "$OBJ"
