#!/usr/bin/env bash

echo "check_format.sh - перевіряє форматування .c та .h файлів проєкту"
echo "Використання: ./check_format.sh <директорія проєкту>"
echo

DIR=$1

if [ -t 1 ]; then
      GREEN='\033[0;32m'
      RED='\033[0;31m'
      RESET='\033[0m'
else
      GREEN=''
      RED=''
      RESET=''
fi

if [ -z "$DIR" ]; then
    echo "Директорія не вказана"
    exit 1
fi

if [ ! -d "$DIR" ]; then
      echo "Директорії $DIR не існує"
      exit 1
fi

if [ ! -f "$DIR/.clang-format" ]; then
      echo "У корені проєкту $DIR немає файлу .clang-format"
      exit 1
fi

for VER in 11 22
do
      echo "==== CLANG-FORMAT $VER VERSION ===="
      BAD=0
      for FILE in $(find "$DIR" -name "*.c" -o -name "*.h" | sort)
      do
            if clang-format-$VER --style=file --dry-run -Werror "$FILE" 2>/dev/null
            then
                    printf "${GREEN}[ OK ]${RESET} %s\n" "$FILE"
            else
                    printf "${RED}[FAIL]${RESET} %s\n" "$FILE"
                    BAD=$((BAD + 1))
            fi
      done

      echo
      echo "Не відформатовано файлів: $BAD"
      echo "================================="
      echo
done
