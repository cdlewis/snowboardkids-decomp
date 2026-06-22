#!/usr/bin/env bash
set -euo pipefail

trap 'echo "BUILD HAS FAILED. Treat this as a build failure; warnings or a non-matching checksum still count as failures."' ERR

QUIET=1 make clean
QUIET=1 make extract >/dev/null
BUILD_AND_VERIFY=1 QUIET=1 make -j8

printf "\033[0;32mBUILD SUCCEEDED\033[0m. The rebuilt ROM matches snowboardkids.sha1.\n"
