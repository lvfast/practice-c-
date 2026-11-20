#!/usr/bin/env bash
set -euo pipefail
root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
lesson="${1:-}"
mode="${2:-test}"
if [[ "$mode" != test && "$mode" != demo && "$mode" != check ]]; then
    echo 'Usage: bash practice/run.sh 01|all test|demo|check' >&2; exit 1
fi
compiler="${CXX:-g++}"
command -v "$compiler" >/dev/null || { echo 'Missing C++ compiler' >&2; exit 1; }
command -v timeout >/dev/null || { echo 'Missing timeout (GNU coreutils)' >&2; exit 1; }
mkdir -p "$root/build"
found=0
overall=0
for directory in "$root"/[0-9][0-9]_*; do
    name="$(basename -- "$directory")"
    [[ "$name" == 11_* ]] && continue
    [[ "$lesson" == all || "$name" == "$lesson" || "$name" == "${lesson}_"* ]] || continue
    found=1
    entry=test.cpp
    [[ "$mode" == demo ]] && entry=main.cpp
    binary="$root/build/${name}_${mode}"
    echo "BUILD $name $mode"
    "$compiler" -std=c++17 -Wall -Wextra -Werror -pthread "$directory/lesson.cpp" "$directory/$entry" -o "$binary"
    set +e
    output="$(cd -- "$directory" && timeout 10s "$binary" 2>&1)"
    code=$?
    set -e
    printf '%s\n' "$output"
    if [[ "$code" == 124 ]]; then echo '[FAIL] TIMEOUT: possible deadlock'; exit 124; fi
    if [[ "$mode" == check ]]; then
        if [[ "$code" != 2 || "$output" != *'[TODO]'* || "$output" == *'[FAIL]'* ]]; then
            echo '[FAIL] Expected untouched TODO scaffold'; exit 1
        fi
        echo 'SCAFFOLD OK (exercise remains TODO)'
    elif [[ "$code" != 0 && "$code" != 2 ]]; then overall=1
    elif [[ "$code" == 2 && "$overall" == 0 ]]; then overall=2
    fi
done
if [[ "$found" == 0 ]]; then echo 'Choose 01..10, a full folder name, or all.' >&2; exit 1; fi
exit "$overall"
