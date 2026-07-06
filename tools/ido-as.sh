#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -ne 4 ]; then
    echo "usage: $0 <input.s> <output.o> <mips2|mips3> <-Oflag>" >&2
    exit 2
fi

input=$1
output=$2
isa=$3
opt=$4

case "$isa" in
    mips2)
        mips_isa=2
        mips_flag=-mips2
        dwopcode=
        ;;
    mips3)
        mips_isa=3
        mips_flag=-mips3
        dwopcode=-dwopcode
        ;;
    *)
        echo "unsupported ISA: $isa" >&2
        exit 2
        ;;
esac

case "$opt" in
    -O0|-O1|-O2|-O3) ;;
    *)
        echo "unsupported optimization flag: $opt" >&2
        exit 2
        ;;
esac

root=$(cd "$(dirname "$0")/.." && pwd)
ido="$root/tools/ido-recomp/linux"
tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

preprocessed="$tmpdir/source.i"
binasm="$tmpdir/source.B"
symtab="$tmpdir/source.T"

c_defs=(
    -D_MIPS_FPSET=16
    -D_MIPS_ISA="$mips_isa"
    -D_ABIO32=1
    -D_MIPS_SIM=_ABIO32
    -D_MIPS_SZINT=32
    -D_MIPS_SZLONG=32
    -D_MIPS_SZPTR=32
    -nostdinc
    -DLANGUAGE_ASSEMBLY
    -D_LANGUAGE_ASSEMBLY
    -D__INLINE_INTRINSICS
    -Dsgi
    -D__sgi
    -Dunix
    -Dmips
    -Dhost_mips
    -D__unix
    -D__host_mips
    -D_SVR4_SOURCE
    -D_MODERN_C
    -D_SGI_SOURCE
    -D__DSO__
    -DSYSTYPE_SVR4
    -D_SYSTYPE_SVR4
    -D_LONGLONG
    -D__mips="$mips_isa"
    -I"$root/include"
    -I"$root/include/compiler/ido"
    -I"$root/include/PR"
    -D_MIPSEB
    -DMIPSEB
    -D__STDC__=1
    -D_MIPS_SZLONG=32
    -DCOMPILING_LIBULTRA
    -DBUILD_VERSION=VERSION_I
    -DBUILD_VERSION_STRING=\"2.0I\"
    -D_FINALROM
    -DNDEBUG
)

"$ido/cfe" "${c_defs[@]}" "$input" -E -std0 \
    -DLANGUAGE_ASSEMBLY -D_LANGUAGE_ASSEMBLY -D__unix -D__host_mips -D__DSO__ \
    -I"$root/include" -I"$root/include/compiler/ido" -I"$root/include/PR" \
    -D_MIPSEB -DMIPSEB -D__STDC__=1 -D_MIPS_SZLONG=32 -DCOMPILING_LIBULTRA \
    -DBUILD_VERSION=VERSION_I -DBUILD_VERSION_STRING=\"2.0I\" -D_FINALROM -DNDEBUG \
    > "$preprocessed"

"$ido/as0" -G 0 -r4300_mul -r4300_mul "$mips_flag" -EB -g0 ${dwopcode:+"$dwopcode"} \
    "$opt" "$preprocessed" -o "$binasm" -t "$symtab"

as1_flags=(-pic0 -elf -G 0 -p0 -r4300_mul -r4300_mul "$mips_flag" -EB -g0)
if [ -n "$dwopcode" ]; then
    as1_flags+=("$dwopcode")
fi
as1_flags+=("$opt")
if [ "$opt" = "-O3" ]; then
    as1_flags+=(-noglobal -Olimit 5000)
fi

"$ido/as1" "${as1_flags[@]}" "$binasm" -o "$output" -t "$symtab"
