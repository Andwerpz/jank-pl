#!/usr/bin/env bash

set -euo pipefail
REPO_ROOT=$(git rev-parse --show-toplevel)
source $REPO_ROOT/scripts/utils.sh

echo -e "${BLUE}Installing the precommit script...${RESET}"
ln -s ${REPO_ROOT}/scripts/git-hooks/precommit/run_tests.sh ${REPO_ROOT}/.git/hooks/pre-commit
chmod +x ${REPO_ROOT}/.git/hooks/pre-commit
echo -e "${GREEN}Precommit script installed!${GREEN}"
