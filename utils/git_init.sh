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

create_config()
{
    echo "Configuration file '$CONFIG_FILE' was not found."
    read -r -p "Create it now? [y/N]: " answer

    case "$answer" in
        y|Y|yes|YES)
            echo
            echo "Enter default Git configuration values."

            read -r -p "User name: " USER_NAME
            read -r -p "User email: " USER_EMAIL
            read -r -p "Default branch [main]: " USER_BRANCH

            if [[ -z "$USER_BRANCH" ]]; then
                USER_BRANCH="main"
            fi

            cat > "$CONFIG_FILE" <<EOF
USER_NAME="$USER_NAME"
USER_EMAIL="$USER_EMAIL"
USER_BRANCH="$USER_BRANCH"
EOF

            chmod 600 "$CONFIG_FILE"

            info "Configuration saved to '$CONFIG_FILE'."
            ;;
        *)
            info "Configuration was not created."
            return 1
            ;;
    esac
}

load_config()
{

    if [[ ! -f "$CONFIG_FILE" ]]; then
        create_config || return 1
    fi

    source "$CONFIG_FILE"

    if [[ -z "${USER_NAME:-}" ||
          -z "${USER_EMAIL:-}" ||
          -z "${USER_BRANCH:-}" ]]; then
        error "Configuration file is incomplete."
        error "Required variables: USER_NAME, USER_EMAIL, USER_BRANCH."
        return 1
    fi
    return 0
}

is_git_repository()
{
    [[ -d "$1/.git" ]]
}

is_directory_empty()
{
    [[ -z "$(find "$1" -mindepth 1 -maxdepth 1 -print -quit 2>/dev/null)" ]]
}

configure_repository()
{
    local repo_dir="$1"

    git -C "$repo_dir" config --local user.name "$USER_NAME" || return 1
    git -C "$repo_dir" config --local user.email "$USER_EMAIL" || return 1
    git -C "$repo_dir" config --local init.defaultBranch "$USER_BRANCH" || return 1

    info "Git local configuration applied."
}

create_repository()
{
    local repo_dir="$1"

    git -C "$repo_dir" init --initial-branch="$USER_BRANCH" ||
        return 1

    configure_repository "$repo_dir" || return 1

    printf '# %s\n' "$(basename "$repo_dir")" > "$repo_dir/README.md"

    info "README.md created."
    info "Repository initialized in '$repo_dir'."

    return 0
}

add_remote()
{
    local repo_dir="$1"
    local remote_url="$2"

    if git -C "$repo_dir" remote get-url origin >/dev/null 2>&1; then
        local current_remote
        current_remote="$(git -C "$repo_dir" remote get-url origin)"

        if [[ "$current_remote" == "$remote_url" ]]; then
            info "Remote 'origin' is already configured."
        else
            info "Remote 'origin' already exists:"
            info "      $current_remote"
            info "Remote was not changed."
        fi
    else
        git -C "$repo_dir" remote add origin "$remote_url" ||
            return 1

        info "Remote 'origin' added."
    fi

    return 0
}


initialize_repository()
{
    local repo_dir="$1"
    local remote_url="${2:-}"

    if [[ ! -e "$repo_dir" ]]; then
        mkdir -p "$repo_dir" || {
            error "Cannot create directory '$repo_dir'."
            return 1
        }

        info "Directory '$repo_dir' created."

        create_repository "$repo_dir" || return 1
    else
        if [[ ! -d "$repo_dir" ]]; then
            error "'$repo_dir' exists and is not a directory."
            return 1
        fi

        if is_git_repository "$repo_dir"; then
            if [[ -n "$remote_url" ]]; then
                info "'$repo_dir' is already a Git repository."
                configure_repository "$repo_dir"
                add_remote "$repo_dir" "$remote_url"
                return $?
            else
                info "'$repo_dir' is already a Git repository."
                info "No changes were made."
                return 0
            fi
        fi

        if is_directory_empty "$repo_dir"; then
            info "Directory '$repo_dir' exists and is empty."

            create_repository "$repo_dir" || return 1
        else
            error "Directory '$repo_dir' already contains files"
            error "and is not a Git repository."
            error "Operation cancelled to prevent data loss."
            return 1
        fi
    fi

    if [[ -n "$remote_url" ]]; then
        add_remote "$repo_dir" "$remote_url" || return 1
    fi

    return 0
}

# ============================
# тут вже виконання буде внизу
# ============================


if [[ -d ".git" ]]; then
    error "Current directory is already a Git repository."
    error "For safety, the script will not continue."
    exit 1
fi

if [[ $# -gt 2 ]]; then
    error "Invalid number of parameters: $#."
    error "Expected 0, 1 or 2 parameters."
    usage
    exit 1
fi

if ! load_config; then
    exit 1
fi


case "$#" in
    0)
        usage
        ;;
    1)
        initialize_repository "$1"
        ;;
    2)
        initialize_repository "$1" "$2"
        ;;
esac
