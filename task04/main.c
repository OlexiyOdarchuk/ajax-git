#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/utsname.h>

void get_time_string(char *buffer, size_t size) {
    time_t timer = time(NULL);
    struct tm *local_time = localtime(&timer);

    if (local_time != NULL) {
        strftime(buffer, size, "%a %b %d %H:%M:%S %Y", local_time);
    } else {
        snprintf(buffer, size, "Невідомий час");
    }
}

int main(int argc, char* argv[])
{
    struct utsname sys_info;
        if (uname(&sys_info) == -1) {
            perror("Помилка отримання системної інформації");
            return 1;
        }


    char time_str[64];
    get_time_string(time_str, sizeof(time_str));

    char info_msg[1024];
    snprintf(info_msg, sizeof(info_msg),
        "Інформація про систему:\n\n"
        "Ім'я хоста: %s\n"
        "Операційна система: %s\n"
        "Архітектура: %s\n"
        "Ядро: %s\n"
        "Час на системі: %s\n",
        sys_info.nodename,
        sys_info.sysname,
        sys_info.machine,
        sys_info.release,
        time_str
    );


    if (argc == 1){
        printf("%s", info_msg);
        return 0;
    }

    const char* filename = argv[1];
    FILE* file = fopen(filename, "r");

    if (file != NULL){
        fclose(file);
        printf("Файл %s вже створено, додаю інформацію в кінець файлу\n", filename);
        file = fopen(filename, "a");
    } else{
        file = fopen(filename, "w");
    }

    if (file == NULL){
        perror("Не вдалося відкрити або створити файл для запису");
        return 1;
    }


    if (fputs(info_msg, file) == EOF){
        fprintf(stderr, "Не вдалося записати інформацію в файл\n");
        fclose(file);
        return 1;
    }
    return 0;
}
