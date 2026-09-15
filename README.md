# [Репозиторій](https://github.com/OlexiyOdarchuk/ajax-git)

~/Desktop/ajax-git 
> git init
Ініціалізовано порожнє Git сховище в /home/ishawyha/Desktop/ajax-git/.git/

ajax-git on git main 
> gh repo create ajax-git --public
✓ Created repository OlexiyOdarchuk/ajax-git on github.com
  https://github.com/OlexiyOdarchuk/ajax-git

ajax-git on git main took 3s 
> touch main.c

ajax-git on git main [?] via C v16.2.1-gcc 
> micro main.c

ajax-git on git main [?] via C v16.2.1-gcc took 27s 
> git add .

ajax-git on git main [+] via C v16.2.1-gcc 
> git commit -m "feat: create main.c"
[main (кореневий коміт) 0aa5ba2] feat: create main.c
 1 file changed, 6 insertions(+)
 create mode 100644 main.c

ajax-git on git main via C v16.2.1-gcc 
> git remote add origin git@github.com:OlexiyOdarchuk/ajax-git.git

ajax-git on git main via C v16.2.1-gcc 
> git branch --set-upstream-to=origin/main main
гілку "main" налаштовано на відстежування "origin/main".

ajax-git on git main [<>] via C v16.2.1-gcc 
> git pull origin main --allow-unrelated-histories
Від github.com:OlexiyOdarchuk/ajax-git
 * branch            main       -> FETCH_HEAD
Merge made by the 'ort' strategy.
 task2.c | 613 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 1 file changed, 613 insertions(+)
 create mode 100644 task2.c

ajax-git on git main [>] via C v16.2.1-gcc took 6s 
> git push --set-upstream origin main
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

ajax-git on git main via C v16.2.1-gcc took 2s 
> touch README.md