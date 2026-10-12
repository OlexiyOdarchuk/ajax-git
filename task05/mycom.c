/*
 * mycom - UART ECHO application
 * Reads lines from a serial port, answers with swapped case, and logs them.
 */

#define _DEFAULT_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

void swap_case(char *s) {
    for (; *s != '\0'; s++) {
        if (isupper((unsigned char)*s)) {
            *s = (char)tolower((unsigned char)*s);
        } else if (islower((unsigned char)*s)) {
            *s = (char)toupper((unsigned char)*s);
        }
    }
}

int setup_uart(int fd, speed_t baud)
{
    struct termios tio;

    if (tcgetattr(fd, &tio) != 0) {
        return -1;
    }

    cfmakeraw(&tio);

    cfsetispeed(&tio, baud);
    cfsetospeed(&tio, baud);

    if (tcsetattr(fd, TCSANOW, &tio) != 0) {
        return -1;
    }
    return 0;
}

int main(int args, char **argv)
{
    char test_string[] = "hello world!\n";
    swap_case(test_string);
    printf("%s", test_string);

    if (args < 2) {
        printf(
            "To use this, specify the path to the UART port: %s /dev/ttyAMA0\n",
            argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("Failed to open port");
        return 1;
    }
    printf("Port opened successfully! File descriptor: %d\n", fd);

    if (setup_uart(fd, B115200) != 0) {
            perror("Failed to configure port");
            close(fd);
            return 1;
        }

    printf("Port %s successfully configured and ready!\n", argv[1]);

    close(fd);
    return 0;
}
