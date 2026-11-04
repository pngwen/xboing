#!/usr/bin/env bash

# Compile-check every implementation file in the raylib port.
# Run from anywhere; all paths are resolved relative to this script.

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

SRC_DIR="$ROOT_DIR/src"
INCLUDE_DIR="$ROOT_DIR/include"
RAYLIB_INCLUDE_DIR="$ROOT_DIR/raylib/src"
GLFW_INCLUDE_DIR="$RAYLIB_INCLUDE_DIR/external/glfw/include"
OUTPUT_DIR="${TMPDIR:-/tmp}/xboing-compile-test"
COMPILER_OUT="$ROOT_DIR/tests/COMPILER.OUT"

mkdir -p "$OUTPUT_DIR"
: > "$COMPILER_OUT"

if [ ! -d "$RAYLIB_INCLUDE_DIR" ]; then
    echo "raylib submodule is not initialized: $RAYLIB_INCLUDE_DIR is missing" >&2
    exit 1
fi

succeed=1
for x in "$SRC_DIR"/*.c; do
    name="$(basename "$x")"
    echo -n "$name... "
    printf '\n\n\n%s\n' "$name" >> "$COMPILER_OUT"
    if gcc -I "$INCLUDE_DIR" -I "$RAYLIB_INCLUDE_DIR" -I "$GLFW_INCLUDE_DIR" \
        -c "$x" -o "$OUTPUT_DIR/${name%.c}.o" 2>> "$COMPILER_OUT"; then
        echo "PASS"
    else
        echo "FAIL"
        succeed=0
    fi
done

if [ "$succeed" -eq 1 ]; then
    echo "All tests succeeded"
else
    echo "Some tests failed."
    exit 1
fi
