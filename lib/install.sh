#!/usr/bin/env bash

set -u

# Paths are relative to the directory from which this script is run.
directories=(
    "jank-stdlib-core"
    "jank-stdlib-containers"
    "jank-stdlib-sys"
    "jank-stdlib-algorithm"
    "jank-stdlib-math"
    "jank-stdlib-crypto"
    "jank-stdlib-io"
    "jank-stdlib-networking"
    "jank-stdlib-sync"
    "jank-stdlib"
    "jank-runtime"
)

root_directory="$PWD"

for directory in "${directories[@]}"; do
    package_path="$root_directory/$directory"

    echo "Installing $directory..."

    if [[ ! -d "$package_path" ]]; then
        echo "ERROR: Directory does not exist: $package_path" >&2
        exit 1
    fi

    output_file="$(mktemp)"

    (
        cd "$package_path" || exit 1
        dylan install
    ) >"$output_file" 2>&1

    status=$?

    if [[ $status -ne 0 ]]; then
        echo "ERROR: Installation failed in '$directory' with status $status." >&2
        echo "----- dylan output -----" >&2
        cat "$output_file" >&2
        rm -f "$output_file"
        exit "$status"
    fi

    rm -f "$output_file"
    echo "Successfully installed $directory."
done

echo "All packages installed successfully."
