#!/usr/bin/env bash
set -euo pipefail

# stdin: JSON with tool_input containing .file_path for Write/Edit tools,
# patch text for apply_patch, or .command/.cmd for shell tools.
input=$(cat)

is_blocked_path() {
  local file_path="$1"

  case "$file_path" in
    *.s|asm/*|./asm/*|*/asm/*)
      echo "Blocked: Agents must not create, edit, move, or delete .s files or files in asm/. Assembly files are generated artifacts; change the corresponding C source or project metadata instead." 1>&2
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

# Block shell mutation primitives when they target an explicit .s path. Read-only
# commands that inspect assembly remain allowed, as do build scripts that create
# their own generated outputs without directly naming those outputs here.
command=$(echo "$input" | jq -r '.tool_input.command? // .tool_input.cmd? // empty')

if [[ -n "$command" ]]; then
  if {
    printf '%s\n' "$command" | grep -Eq \
      '(^|[;&|[:space:]])([0-9]*>{1,2}|tee([[:space:]]+-[^[:space:]]+)*|touch|truncate|rm|unlink|mv|cp|install|chmod|chown|sed[[:space:]]+-[^[:space:]]*i|perl[[:space:]]+-[^[:space:]]*i)([[:space:]]|[^;&|]*[[:space:]])[^;&|]*\.s([^[:alnum:]_]|$)' ||
    { printf '%s\n' "$command" | grep -Eq '(^|[;&|[:space:]])apply_patch([[:space:]]|$)' &&
      printf '%s\n' "$command" | grep -Eq '\.s([^[:alnum:]_]|$)'; } ||
    printf '%s\n' "$command" | grep -Eq \
      '(^|[;&|[:space:]])git[[:space:]]+(checkout|restore|clean|apply)([[:space:]]|[^;&|]*[[:space:]])[^;&|]*\.s([^[:alnum:]_]|$)' ||
    { printf '%s\n' "$command" | grep -Eq '(^|[;&|[:space:]])(python3?|ruby|node)[[:space:]]+(-c|-e)([[:space:]]|$)' &&
      printf '%s\n' "$command" | grep -Eq '\.s.*(write|unlink|remove|rename|replace|open\([^)]*,[[:space:]]*["'\'']?[wax+])'; }
  }; then
    echo "Blocked: Agents must not create, edit, move, or delete .s files. Read them for analysis or run the repository's build tools instead." 1>&2
    exit 2
  fi
fi

exit 0
