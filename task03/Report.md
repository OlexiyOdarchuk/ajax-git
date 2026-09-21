# TASK 03

## Середовище

Дві версії clang-format з максимальною різницею:

```console
$ clang-format-11 --version
clang-format version 11.1.0
$ clang-format-22 --version
clang-format version 22.1.8
```

XX = 11, YY = 22, різниця 11 релізів LLVM.

## Проєкт dummy

8 файлів з ядра Linux, разом 847 КБ:

```bash
task03/dummy/.clang-format     # з кореня репозиторія ядра
task03/dummy/src/              # namei.c, dir.c, random.c, tty_io.c, core.c
task03/dummy/include/          # fs.h, sched.h, mm.h
```

Файли бралися з `fs/`, `drivers/` та `include/linux/`

## Вибір версій

Спочатку пробував взяти версію 10, щоб різниця була ще більшою. Виявилось,
що вона не читає конфіг ядра:

```console
$ clang-format-10 --style=file --dry-run src/dir.c
YAML:796:20: error: unknown enumerated scalar
SpaceBeforeParens: ControlStatementsExceptForEachMacros
                   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Error reading .../dummy/.clang-format: Invalid argument
```

Опція `SpaceBeforeParens: ControlStatementsExceptForEachMacros` з'явилась у
версії 11, і в шапці конфігу так і написано - "Intended for clang-format >= 11".

Тому XX = 11: це найстаріша версія, яка ще розуміє актуальний конфіг ядра.

## Перевірка форматування

TODO

## Вплив версії форматера

TODO

## Різниця між Chromium-конфігами версій

TODO

## Перехід на інший формат

TODO

## Перехід на нову версію формату

TODO

## Висновки

TODO
