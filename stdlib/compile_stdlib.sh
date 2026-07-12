#!/usr/bin/env bash

set -euo pipefail

SRC_DIR="./src"
ASM_DIR="./asm"
LOG_FILE="$(mktemp)"

trap 'rm -f "$LOG_FILE"' EXIT

find "$SRC_DIR" -type f -name '*.jank' -print0 |
while IFS= read -r -d '' src_file; do
    relative_path="${src_file#"$SRC_DIR"/}"
    output_file="$ASM_DIR/${relative_path%.jank}.s"

    mkdir -p "$(dirname "$output_file")"

    printf 'jjc %q -S -o %q\n' "$src_file" "$output_file"

    if ! jjc "$src_file" -S -o "$output_file" >"$LOG_FILE" 2>&1; then
        echo "Failed to compile: $src_file" >&2
        echo "Last 50 lines:" >&2
        tail -n 50 "$LOG_FILE" >&2
        exit 1
    fi

    : >"$LOG_FILE"
done