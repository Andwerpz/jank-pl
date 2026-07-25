#!/usr/bin/env bash

set -euo pipefail
trap cleanup EXIT

RED='\e[31m'
GREEN='\e[32m'
YELLOW='\e[0;33m'
BLUE='\e[0;34m'
RESET='\e[0m'

REPO_ROOT=$(git rev-parse --show-toplevel)
CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
CHANGED_FILES=$(git -C "$REPO_ROOT" diff --cached --name-only)
WATCHED_DIRS="
compiler/src/
stdlib/src/
testing/tests/
testing/main.cpp
"

cleanup() {
    echo -e "${BLUE}Bai bai Mr Li!${RESET}\n"
}

build_compiler() {
    cd "${REPO_ROOT}/compiler"

    echo -e "${BLUE}Building compiler...${RESET}"
    if make -s; then
        echo -e "${GREEN}Compiler built!${RESET}"
    else
        echo -e "${RED}Compiler not working... Ask Sanat Dubey @(512) 995-9950!${RESET}"
        return 1
    fi
    echo ""
}

run_tests() {
    cd "${REPO_ROOT}/testing"

    echo -e "${BLUE}Building test runner...${RESET}"
    if make -s; then
        echo -e "${GREEN}Test runner built!${RESET}"
    else
        echo -e "${RED}Test runner not working... Ask Sanat Dubey @(512) 995-9950!${RESET}"
        return 1
    fi
    echo ""

    echo -e "${BLUE}Running tests...${RESET}"
    if make -s test; then
        echo -e "${GREEN}Good work big man!${RESET}"
    else
        echo -e "${RED}Not working big man! Ask Sanat Dubey @(512) 995-9950!${RESET}"
        return 1
    fi
    echo ""
}

echo -e "${BLUE}Bello guys! My name is Dylan Janky! I love Andrew Mc Li!${RESET}\n"
echo -e "${YELLOW}Running precommit tests on branch: '${CURRENT_BRANCH}'${RESET}"
for dir in $WATCHED_DIRS; do
    if echo "$CHANGED_FILES" | grep -q "^$dir"; then
        echo -e "${YELLOW}Changes detected. Running tests...${RESET}\n"
        if build_compiler && run_tests; then
            exit 0
        else
            exit 1
        fi
    fi
done

echo -e "${YELLOW}No watched directory changes. Skipping tests.${RESET}\n"
exit 0
