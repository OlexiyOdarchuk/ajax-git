# ajax-git

Репозиторій на GitHub: [OlexiyOdarchuk/ajax-git](https://github.com/OlexiyOdarchuk/ajax-git)

Покроковий журнал створення репозиторію: від `git init` до синхронізації з GitHub.

## Зміст

- [ajax-git](#ajax-git)
  - [Зміст](#зміст)
  - [1. Ініціалізація локального репозиторію](#1-ініціалізація-локального-репозиторію)
  - [2. Створення репозиторію на GitHub](#2-створення-репозиторію-на-github)
  - [3. Перший коміт](#3-перший-коміт)
  - [4. Підключення віддаленого репозиторію](#4-підключення-віддаленого-репозиторію)
  - [5. Злиття з віддаленою історією](#5-злиття-з-віддаленою-історією)
  - [6. Відправлення змін на GitHub](#6-відправлення-змін-на-github)
  - [7. Додавання README.md та .gitignore](#7-додавання-readmemd-та-gitignore)

---

## 1. Ініціалізація локального репозиторію

Створює в поточній директорії порожній Git-репозиторій (приховану папку `.git`).

```console
$ git init
Ініціалізовано порожнє Git сховище в /home/ishawyha/Desktop/ajax-git/.git/
```

## 2. Створення репозиторію на GitHub

Через GitHub CLI створюється публічний репозиторій `ajax-git` в обліковому записі на GitHub.

```console
$ gh repo create ajax-git --public
✓ Created repository OlexiyOdarchuk/ajax-git on github.com
  https://github.com/OlexiyOdarchuk/ajax-git
```

## 3. Перший коміт

Створюється файл `main.c`, редагується в `micro`, додається в індекс і фіксується першим (кореневим) комітом.

```console
$ touch main.c
$ micro main.c
$ git add .
$ git commit -m "feat: create main.c"
[main (кореневий коміт) 0aa5ba2] feat: create main.c
 1 file changed, 6 insertions(+)
 create mode 100644 main.c
```

## 4. Підключення віддаленого репозиторію

Локальний репозиторій звʼязується з GitHub (`origin`), а локальна гілка `main` налаштовується на відстежування `origin/main`.

```console
$ git remote add origin git@github.com:OlexiyOdarchuk/ajax-git.git
$ git branch --set-upstream-to=origin/main main
гілку "main" налаштовано на відстежування "origin/main".
```

## 5. Злиття з віддаленою історією

Локально створив `main.c`, а на GitHub створив окремий коміт із файлом `task2.c`. Локальна і віддалена історії не мають спільного предка, тому звичайний `git pull` відмовляється їх обʼєднувати. Прапорець `--allow-unrelated-histories` дозволяє таке злиття — Git створює merge-коміт.

```console
$ git pull origin main --allow-unrelated-histories
Від github.com:OlexiyOdarchuk/ajax-git
 * branch            main       -> FETCH_HEAD
Merge made by the 'ort' strategy.
 task2.c | 613 +++++++++++++++++++++++++++++++++++++++++++
 1 file changed, 613 insertions(+)
 create mode 100644 task2.c
```

## 6. Відправлення змін на GitHub

Обʼєднана історія відправляється на GitHub, після чого локальна і віддалена гілки `main` збігаються.

```console
$ git push --set-upstream origin main
Перерахування обʼєктів: 6, готово.
Підрахунок обʼєктів: 100% (6/6), готово.
Дельта компресія з використанням до 22 потоків
Компресія обʼєктів: 100% (4/4), готово.
Запис обʼєктів: 100% (5/5), 867 байтів | 867.00 КіБ/с, готово.
Всього 5 (дельта 1), повторно використано 0 (дельта 0), повторно використано пакунків 0 (з 0)
remote: Resolving deltas: 100% (1/1), done.
To github.com:OlexiyOdarchuk/ajax-git.git
   6556cac..d539d7e  main -> main
гілку "main" налаштовано на відстежування "origin/main".
```

## 7. Додавання README.md та .gitignore

Створюється цей файл документації, а також `.gitignore`, щоб не потрапляли в репозиторій артефакти збірки.

```console
$ touch README.md
```
