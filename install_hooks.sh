#!/usr/bin/env bash

set -euo pipefail

REPO_ROOT=$(git rev-parse --show-toplevel)

ln -s ${REPO_ROOT}/scripts/git-hooks/precommit/run_tests.sh ${REPO_ROOT}/.git/hooks/pre-commit
chmod +x ${REPO_ROOT}/.git/hooks/pre-commit
