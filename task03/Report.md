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

`check_format.sh` приймає корінь проєкту і перевіряє всі `.c` та `.h` двома
версіями. Перевірка кожного файлу - через `--dry-run -Werror` і код завершення:
`0` означає, що файл відповідає конфігу, `1` - що ні.

Помилкові аргументи відсікаються:

```console
$ ./check_format.sh ./nope
Директорії ./nope не існує
$ ./check_format.sh ./dummy/src
У корені проєкту ./dummy/src немає файлу .clang-format
```

Результат:

```console
$ ./check_format.sh ./dummy
==== CLANG-FORMAT 11 VERSION ====
[FAIL] ./dummy/include/fs.h
[FAIL] ./dummy/include/mm.h
...
Не відформатовано файлів: 8
=================================

==== CLANG-FORMAT 22 VERSION ====
[FAIL] ./dummy/include/fs.h
...
Не відформатовано файлів: 8
=================================
```

Жоден з 8 файлів не проходить перевірку, і це нормально. `.clang-format` в
ядрі описує цільовий стиль, а не фактичний стан дерева - код там пишуть люди,
і автоматично весь репозиторій ніхто не переформатовує.

## Вплив версії форматера

Спочатку прогнав `make_format_11.sh`, закомітив, потім `make_format_22.sh` і ще
раз закомітив. Конфіг обидва рази той самий, змінювалась тільки версія - тому
все, що дав другий прогін, це чиста різниця між версіями.

```console
$ ./make_format_11.sh ./dummy
$ git diff --shortstat
 8 files changed, 3235 insertions(+), 2908 deletions(-)

$ ./make_format_22.sh ./dummy
$ git diff --shortstat
 6 files changed, 34 insertions(+), 43 deletions(-)
```

Перше форматування зачепило всі 8 файлів і переписало більше 3000 рядків.
Друге - 34 рядки в 6 файлах, майже в сто разів менше. Але ці 34 рядки нікуди
не зникають, і ось чому.

Перевірка після кожного кроку:

| Після чого | 11 бачить проблем | 22 бачить проблем |
| --- | --- | --- |
| завантаження з ядра | 8 файлів | 8 файлів |
| make_format_11 | 0 | 6 файлів |
| make_format_22 | 6 файлів | 0 |

Файли ті самі. Кожна версія вважає правильним тільки власний результат, і
жодна не погоджується з чужим. Якщо в команді в одних розробників стара версія,
а в інших нова, ці 6 файлів перекидатимуться туди-сюди після кожного коміту, і
кожен буде "виправляти" за попереднім.

Що саме не поділили версії (`-` це 11, `+` це 22):

Ініціалізатори в макросах 22-га стискає в один рядок:

```c
-#define TLB_FLUSH_VMA(mm, flags)                   \
-	{                                          \
-		.vm_mm = (mm), .vm_flags = (flags) \
-	}
+#define TLB_FLUSH_VMA(mm, flags) { .vm_mm = (mm), .vm_flags = (flags) }
```

Відступи в перенесених рядках у 22-ї менші:

```c
-			     NULL :
-			     p4d_offset(pgd, address);
+		       NULL :
+		       p4d_offset(pgd, address);
```

22-га виправила розбір `&` після дужки - раніше форматер вважав його взяттям
адреси, тепер бачить бінарну операцію:

```c
-#define ACC_MODE(x) ("\004\002\006\006"[(x)&O_ACCMODE])
+#define ACC_MODE(x) ("\004\002\006\006"[(x) & O_ACCMODE])
```

А тут навпаки - 22-га прийняла приведення типу за віднімання:

```c
-#define DIRECT_MAP_PHYSMEM_END (((phys_addr_t)-1) & ~(1ULL << 63))
+#define DIRECT_MAP_PHYSMEM_END (((phys_addr_t) - 1) & ~(1ULL << 63))
```

Два останні приклади поруч показують головне: новіша версія не означає
правильніша. Один клас помилок розбору виправили, інший з'явився.

## Різниця між Chromium-конфігами версій

TODO

## Перехід на інший формат

TODO

## Перехід на нову версію формату

TODO

## Висновки

TODO
