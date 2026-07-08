#!/usr/bin/env bash
set -euo pipefail

# stdin: JSON with tool_input containing .file_path for Write/Edit tools,
# or patch text for apply_patch.
input=$(cat)

is_blocked_path() {
  local file_path="$1"

  case "$file_path" in
    asm/*|./asm/*|*/asm/*)
      echo "Blocked: Files in asm/ are automatically generated and should not be edited directly. To rename a symbol, update symbol_addrs.txt. To regenerate the assembly files, run ./tools/build-and-verify.sh." 1>&2
      exit 2
      ;;
  esac
}

# Check for direct file path fields in Write/Edit-style tools.
while IFS= read -r file_path; do
  [[ -n "$file_path" ]] && is_blocked_path "$file_path"
done < <(echo "$input" | jq -r '.tool_input.file_path?, .tool_input.path? // empty')

# Check apply_patch-style freeform patch bodies.
patch_text=$(echo "$input" | jq -r '
  if (.tool_input | type) == "string" then
    .tool_input
  else
    .tool_input.patch? // .tool_input.input? // empty
  end
')

while IFS= read -r file_path; do
  [[ -n "$file_path" ]] && is_blocked_path "$file_path"
done < <(
  printf '%s\n' "$patch_text" |
    sed -n \
      -e 's/^\*\*\* Add File: //p' \
      -e 's/^\*\*\* Delete File: //p' \
      -e 's/^\*\*\* Update File: //p' \
      -e 's/^\*\*\* Move to: //p' \
      -e 's/^--- a\///p' \
      -e 's/^+++ b\///p'
)

exit 0
