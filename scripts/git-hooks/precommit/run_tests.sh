#!/usr/bin/env bash

set -euo pipefail
REPO_ROOT=$(git rev-parse --show-toplevel)
source $REPO_ROOT/scripts/utils.sh

CHANGED_FILES=$(git -C "$REPO_ROOT" diff --cached --name-only)
WATCHED_DIRS="
compiler/src/
stdlib/src/
testing/tests/
testing/main.cpp
"
REQUIRED_DEPENDENCIES="
${COMPILER_NAME}
${BUILD_TOOL_NAME}
"

echo -e "${YELLOW}Running precommit tests on branch: '${CURRENT_BRANCH}'${RESET}"
echo -e "${BLUE}Bello guys! My name is Dylan Janky! I love Andrew Mc Li!${RESET}"

echo -e "${YELLOW}Checking for required jank dependencies...${RESET}"
for bin in $REQUIRED_DEPENDENCIES; do
    if ! check_command_exists "${bin}"; then
        sanat_dubey_error "Missing dependency: [${bin}]!"
        exit 1
    fi
done
echo ""

echo -e "${YELLOW}Updating compiler and build tool...${RESET}"
make_compiler "-s"
make_dylan "-s"

for dir in $WATCHED_DIRS; do
    if echo "$CHANGED_FILES" | grep -q "^$dir"; then
        echo -e "${YELLOW}Changes detected. Running tests...${RESET}\n"
        if install_stdlib && make_tests "-s" && make_tests "-s test"; then
            exit 0
        else
            exit 1
        fi
    fi
done

echo -e "${YELLOW}No watched directory changes. Skipping tests.${RESET}\n"
exit 0
