#!/usr/bin/env bash
set -euo pipefail

shift_after="${SHIFT_AFTER:-main}"
shift_pad_size="${SHIFT_PAD_SIZE:-0x10}"

QUIET=1 make shift-test SHIFT_AFTER="$shift_after" SHIFT_PAD_SIZE="$shift_pad_size"
