#!/usr/bin/env bash

set -u

CONFIG_FILE=".git_myconfig"
SCRIPT_NAME="$(basename "$0")"

info()
{
    echo "[INFO] $1"
}

error()
{
    echo "[ERROR] $1" >&2
}

usage()
{
    cat <<EOF
Usage:
    $SCRIPT_NAME
    $SCRIPT_NAME <directory>
    $SCRIPT_NAME <directory> <remote>

Examples:
    $SCRIPT_NAME
    $SCRIPT_NAME new_git
    $SCRIPT_NAME new_git https://github.com/user/remote_repo.git

Description:
    Without parameters:
        Show this help.

    With one parameter:
        Create and initialize a local Git repository in the
        specified directory.

    With two parameters:
        Create/initialize the specified directory and add
        the specified remote repository.
EOF
}

load_config()
{
    source "$CONFIG_FILE"

    if [[ -z "${USER_NAME:-}" ||
          -z "${USER_EMAIL:-}" ||
          -z "${USER_BRANCH:-}" ]]; then
        error "Configuration file is incomplete."
        error "Required variables: USER_NAME, USER_EMAIL, USER_BRANCH."
        return 1
    fi
    info "All is good"
    return 0
}

load_config
