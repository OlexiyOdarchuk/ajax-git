/*
 * mycom - UART ECHO application
 * Reads lines from a serial port, answers with swapped case, and logs them.
 */

#define _DEFAULT_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

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

    close(fd);
}
