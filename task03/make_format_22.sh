#!/usr/bin/env bash

echo "make_format_22.sh - форматує .c та .h файли проєкту (clang-format 22)"
echo "Використання: ./make_format_22.sh <директорія проєкту>"
echo

DIR=$1

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

for FILE in $(find "$DIR" -name "*.c" -o -name "*.h" | sort)
do
      clang-format-22 --style=file -i "$FILE"
      echo "formatted $FILE"
done
