#!/usr/bin/env bash

set -euo pipefail
REPO_ROOT=$(git rev-parse --show-toplevel)
source $REPO_ROOT/scripts/utils.sh

# Installs:
# compiler - jjc
# build_tool - dylan
# stdlib

echo -e "${YELLOW}Installing requires sudo priviledges. Please allow (heh).${RESET}"
sudo -v
echo -e "${YELLOW}==============================${RESET}"


echo -e "${BLUE}Building and installing the compiler (jjc)...${RESET}"
make_compiler ""
make_compiler "install"
echo -e "${GREEN}Compiler installed!${RESET}"
echo -e "${YELLOW}==============================${RESET}"

echo -e "${BLUE}Building and installing the build tool (dylan)...${RESET}"
make_dylan ""
make_dylan "install"
echo -e "${GREEN}Build tool installed!${RESET}"
echo -e "${YELLOW}==============================${RESET}"

echo -e "${BLUE}Installing the standard library...${RESET}"
install_stdlib
echo -e "${GREEN}Standard library installed!${RESET}"
echo -e "${YELLOW}==============================${RESET}"
