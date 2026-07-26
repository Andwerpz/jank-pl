RED='\e[31m'
GREEN='\e[32m'
YELLOW='\e[0;33m'
BLUE='\e[0;34m'
RESET='\e[0m'

trap 'cleanup' EXIT
trap 'error_handler' ERR

REPO_ROOT=$(git rev-parse --show-toplevel)
CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD)

error_handler() {
    local exit_code=$?

    sanat_dubey_error "Command failed with exit code ${exit_code}: ${BASH_COMMAND}"
}

cleanup() {
    echo -e "${BLUE}Bai bai Mr Li!${RESET}\n"
}

check_command_exists() {
    local bin=$1

    if command -v "$bin" > /dev/null 2>&1; then
        return 0
    else
        return 1
    fi
}

sanat_dubey_error() {
    local msg=$1

    echo -e "${RED}Error: ${msg}${RESET}"
    echo -e "${RED}For help, ask Sanat Dubey @(512) 995-9950!${RESET}"
}

make_dylan() {
    local flags="$1"

    cd "${REPO_ROOT}/build_tool"

    echo -e "${BLUE}Running build tool Makefile with flags: [${flags}]...${RESET}"
    if make ${flags}; then
        echo -e "${GREEN}Success!${RESET}"
    else
       sanat_dubey_error "Build tool Makefile failed!"
        return 1
    fi
    echo ""
}

make_compiler() {
    local flags="$1"

    cd "${REPO_ROOT}/compiler"

    echo -e "${BLUE}Running compiler Makefile with flags: [${flags}]...${RESET}"
    if make ${flags}; then
        echo -e "${GREEN}Success!${RESET}"
    else
        sanat_dubey_error "Compiler Makefile failed!"
        return 1
    fi
    echo ""
}

install_stdlib() {
    cd "${REPO_ROOT}/stdlib"

    echo -e "${BLUE}Installing standard libraries...${RESET}"
    if dylan install --user; then
        echo -e "${GREEN}Installed standard libraries!${RESET}"
    else
        sanat_dubey_error "Standard libraries failed to install!"
        return 1
    fi
    echo ""
}

make_tests() {
    local flags="$1"

    cd "${REPO_ROOT}/testing"

    echo -e "${BLUE}Running tests Makefile with flags: [${flags}]...${RESET}"
    if make ${flags}; then
        echo -e "${GREEN}Success!${RESET}"
    else
        sanat_dubey_error "Test runner not working..."
        return 1
    fi
    echo ""
}
