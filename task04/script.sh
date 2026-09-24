#!/usr/bin/env bash

SRC="main.c"
TARGET_CC="aarch64-linux-gnu-gcc"

if [ -t 1 ]; then
    GREEN='\033[0;32m'
    RED='\033[0;31m'
    RESET='\033[0m'
else
    GREEN=''
    RED=''
    RESET=''
fi

build_host() {
    echo -e "\nКомпіляція для HOST (Цей ПК)\n"
    gcc -Wall -Wextra "$SRC" -o sysinfo_host
    if [ $? -eq 0 ]; then
        echo -e "$GREEN[+]$RESET Успішно! Створено файл: sysinfo_host"
    else
        echo -e "$RED[-]$RESET Помилка компіляції для Host!"
        exit 1
    fi
}

build_target() {
    echo -e "\nКрос-компіляція для TARGET (Raspberry Pi 5)\n"

    if ! command -v $TARGET_CC &> /dev/null; then
        echo -e "$RED[-]$RESET Крос-компілятор $TARGET_CC не знайдено!"
        exit 1
    fi

    $TARGET_CC -Wall -Wextra "$SRC" -o sysinfo_rpi5
    if [ $? -eq 0 ]; then
        echo -e "$GREEN[+]$RESET Успішно! Створено файл: sysinfo_rpi5"
    else
        echo -e "$RED[-]$RESET Помилка крос-компіляції для Raspberry Pi 5!"
        exit 1
    fi
}

CHOICE=$1

if [ -z "$CHOICE" ]; then
    echo "Виберіть платформу для компіляції:"
    echo "1) Host (Цей ПК)"
    echo "2) Target (Raspberry Pi 5)"
    read -p "Ваш вибір (1 або 2): " CHOICE
fi

case $CHOICE in
    1|host|HOST)
        build_host
        ;;
    2|target|TARGET)
        build_target
        ;;
    *)
        echo "Некоректний вибір. Використовуйте '1' (host) або '2' (target)."
        exit 1
        ;;
esac
