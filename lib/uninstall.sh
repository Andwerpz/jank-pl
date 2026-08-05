#!/usr/bin/env bash

set -u

# Uninstall in reverse order of installation.
directories=(
    "jank-runtime"
    "jank-stdlib"
    "jank-stdlib-sync"
    "jank-stdlib-networking"
    "jank-stdlib-io"
    "jank-stdlib-crypto"
    "jank-stdlib-math"
    "jank-stdlib-algorithm"
    "jank-stdlib-sys"
    "jank-stdlib-containers"
    "jank-stdlib-core"
)

root_directory="$PWD"

for directory in "${directories[@]}"; do
    package_path="$root_directory/$directory"

    echo "Uninstalling $directory..."

    if [[ ! -d "$package_path" ]]; then
        echo "ERROR: Directory does not exist: $package_path" >&2
        exit 1
    fi

    output_file="$(mktemp)"

    (
        cd "$package_path" || exit 1
        dylan uninstall
    ) >"$output_file" 2>&1

    status=$?

    if [[ $status -ne 0 ]]; then
        echo "ERROR: Uninstallation failed in '$directory' with status $status." >&2
        echo "----- dylan output -----" >&2
        cat "$output_file" >&2
        rm -f "$output_file"
        exit "$status"
    fi

    rm -f "$output_file"
    echo "Successfully uninstalled $directory."
done

echo "All packages uninstalled successfully."
