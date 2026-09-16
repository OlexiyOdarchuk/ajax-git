#!/usr/bin/bash

set -u

SCRIPT_NAME="$(basename "$0")"

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
