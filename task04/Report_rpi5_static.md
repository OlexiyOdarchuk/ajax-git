# Звіт: статична компіляція на TARGET (Raspberry Pi 5)

## 1. Компіляція
```text
$ ./script.sh 2 2
```

## 2. Перевірка на працездатність
```text
$ ./sysinfo_rpi5_static
Інформація про систему:

Ім'я хоста: iShawyha-rpi
Операційна система: Linux
Архітектура: aarch64
Ядро: 6.18.50+rpt-rpi-2712
Час на системі: Thu Oct 01 00:50:34 2026
$ ./sysinfo_rpi5_static file.txt
$ ./sysinfo_rpi5_static file.txt
Файл file.txt вже створено, додаю інформацію в кінець файлу
$ cat file.txt
Інформація про систему:

Ім'я хоста: iShawyha-rpi
Операційна система: Linux
Архітектура: aarch64
Ядро: 6.18.50+rpt-rpi-2712
Час на системі: Thu Oct 01 00:50:39 2026
Інформація про систему:

Ім'я хоста: iShawyha-rpi
Операційна система: Linux
Архітектура: aarch64
Ядро: 6.18.50+rpt-rpi-2712
Час на системі: Thu Oct 01 00:50:40 2026
```

## 3. Розмір файлу (ls -l)
```text
-rwxrwxr-x 1 ishawyha ishawyha 849216 Sep 30 18:35 sysinfo_rpi5_static
```

## 4. Розміри секцій пам'яті (size)
```text
   text      data       bss       dec       hex    filename
 665160     24340     22112    711612     adbbc    sysinfo_rpi5_static
```

## 5. Динамічні залежності (ldd)
```text
	not a dynamic executable
```

## 6. Заголовок ELF-файлу (readelf -h)
```text
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 03 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - GNU
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x400740
  Start of program headers:          64 (bytes into file)
  Start of section headers:          847680 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         7
  Size of section headers:           64 (bytes)
  Number of section headers:         24
  Section header string table index: 23
```

## 7. Знайдені текстові рядки (strings)
```text
  Невідомий час
  Помилка отримання системної інформації
  Інформація про систему:
  Ім'я хоста: %s
  Операційна система: %s
  Архітектура: %s
  Ядро: %s
  Час на системі: %s
  Файл %s вже створено, додаю інформацію в кінець файлу
  Не вдалося відкрити або створити файл для запису
  Не вдалося записати інформацію в файл
```

## 8. Статична крос-компіляція на HOST
```text
$ ./script.sh 2 2
$ scp sysinfo_rpi5_static ishawyha@192.168.1.108:~/Desktop/
```

Перевірка на працездатність (на TARGET):
```text
$ ./sysinfo_rpi5_static
Інформація про систему:

Ім'я хоста: iShawyha-rpi
Операційна система: Linux
Архітектура: aarch64
Ядро: 6.18.50+rpt-rpi-2712
Час на системі: Thu Oct 01 00:55:00 2026
$ ./sysinfo_rpi5_static file.txt
$ ./sysinfo_rpi5_static file.txt
Файл file.txt вже створено, додаю інформацію в кінець файлу
$ cat file.txt
Інформація про систему:

Ім'я хоста: iShawyha-rpi
Операційна система: Linux
Архітектура: aarch64
Ядро: 6.18.50+rpt-rpi-2712
Час на системі: Thu Oct 01 00:55:07 2026
Інформація про систему:

Ім'я хоста: iShawyha-rpi
Операційна система: Linux
Архітектура: aarch64
Ядро: 6.18.50+rpt-rpi-2712
Час на системі: Thu Oct 01 00:55:08 2026
```

ls -l:
```text
-rwxr-xr-x 1 ishawyha ishawyha 1054952 Oct  1 00:53 sysinfo_rpi5_static
```

size:
```text
   text      data       bss       dec       hex    filename
 638364     23004     22208    683576     a6e38    sysinfo_rpi5_static
```

ldd:
```text
	not a dynamic executable
```

readelf -h:
```text
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 03 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - GNU
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x400700
  Start of program headers:          64 (bytes into file)
  Start of section headers:          1052840 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         7
  Size of section headers:           64 (bytes)
  Number of section headers:         33
  Section header string table index: 32
```

strings: рядки ідентичні п. 7.

## 9. Порівняння
| Параметр | Динамічна (TARGET) | Статична (TARGET) | Статична (HOST, крос) |
| --- | --- | --- | --- |
| Компілятор | GCC 14.2.0 | GCC 14.2.0 | GCC 16.1.0 |
| Розмір файлу | 70984 | 849216 | 1054952 |
| text / data / bss | 3571 / 712 / 8 | 665160 / 24340 / 22112 | 638364 / 23004 / 22208 |
| Тип | DYN (PIE) | EXEC | EXEC |
| OS/ABI | UNIX - System V | UNIX - GNU | UNIX - GNU |
| Точка входу | 0x980 | 0x400740 | 0x400700 |
| Заголовків програми | 10 | 7 | 7 |
| Заголовків секцій | 29 | 24 | 33 |
| ldd | libc.so.6, ld-linux-aarch64.so.1 | not a dynamic executable | not a dynamic executable |
| Результат роботи | однаковий | однаковий | однаковий |

Статична крос-версія більша через 8 секцій `.debug_*` (налагоджувальна інформація) з `libc.a` крос-тулчейна.
