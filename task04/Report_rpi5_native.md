# Звіт: компіляція на TARGET (Raspberry Pi 5)

## 1. Компіляція
```text
$ ./script.sh 2

Крос-компіляція для TARGET (Raspberry Pi 5)

[+] Успішно! Створено файл: sysinfo_rpi5
```

## 2. Перевірка на працездатність
```text
$ ./sysinfo_rpi5
Інформація про систему:

Ім'я хоста: iShawyha-rpi
Операційна система: Linux
Архітектура: aarch64
Ядро: 6.18.50+rpt-rpi-2712
Час на системі: Wed Sep 30 18:19:14 2026
$ ./sysinfo_rpi5 file.txt
$ ./sysinfo_rpi5 file.txt
Файл file.txt вже створено, додаю інформацію в кінець файлу
$ cat file.txt
Інформація про систему:

Ім'я хоста: iShawyha-rpi
Операційна система: Linux
Архітектура: aarch64
Ядро: 6.18.50+rpt-rpi-2712
Час на системі: Wed Sep 30 18:19:14 2026
Інформація про систему:

Ім'я хоста: iShawyha-rpi
Операційна система: Linux
Архітектура: aarch64
Ядро: 6.18.50+rpt-rpi-2712
Час на системі: Wed Sep 30 18:19:14 2026
```

## 3. Розміри секцій пам'яті (size)
```text
   text	   data	    bss	    dec	    hex	filename
   3571	    712	      8	   4291	   10c3	sysinfo_rpi5
```

## 4. Динамічні залежності (ldd)
```text
	linux-vdso.so.1 (0x00007fff0a1c8000)
	libc.so.6 => /lib/aarch64-linux-gnu/libc.so.6 (0x00007fff09f80000)
	/lib/ld-linux-aarch64.so.1 (0x00007fff0a190000)
```

## 5. Заголовок ELF-файлу (readelf -h)
```text
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Position-Independent Executable file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x980
  Start of program headers:          64 (bytes into file)
  Start of section headers:          69128 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         10
  Size of section headers:           64 (bytes)
  Number of section headers:         29
  Section header string table index: 28
```

## 6. Знайдені текстові рядки (strings)
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

## 7. Порівняння з крос-компіляцією (п. 3)
| Параметр | HOST (крос) | TARGET |
| --- | --- | --- |
| Компілятор | GCC 16.1.0 | GCC 14.2.0 (Debian) |
| Розмір файлу | 71184 | 70984 |
| text / data / bss | 3559 / 712 / 8 | 3571 / 712 / 8 |
| Точка входу | 0x980 | 0x980 |
| Заголовків програми | 10 | 10 |
| Заголовків секцій | 30 | 29 |
| Потрібна glibc | GLIBC_2.17, GLIBC_2.34 | GLIBC_2.17, GLIBC_2.34 |
| ldd | libc.so.6, ld-linux-aarch64.so.1 | libc.so.6, ld-linux-aarch64.so.1 |
