/*
 * mycom - UART ECHO application
 * Reads lines from a serial port, answers with swapped case, and logs them.
 */

#define _DEFAULT_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

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
