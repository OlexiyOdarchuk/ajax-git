/*
 * mycom - UART ECHO application
 * Reads lines from a serial port, answers with swapped case, and logs them.
 */

#define _DEFAULT_SOURCE

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

#define LINE_MAX_LEN 256

volatile sig_atomic_t g_stop = 0;

void on_signal(int sig) {
    (void)sig;
    g_stop = 1;
}

void swap_case(char *s)
{
    for (; *s != '\0'; s++) {
        if (isupper((unsigned char)*s)) {
            *s = (char)tolower((unsigned char)*s);
        } else if (islower((unsigned char)*s)) {
            *s = (char)toupper((unsigned char)*s);
        }
    }
}

int setup_uart(int fd, speed_t baud, struct termios *saved) {
    struct termios tio;

    if (tcgetattr(fd, saved) != 0) {
        return -1;
    }

    tio = *saved;
    cfmakeraw(&tio);

    tio.c_cflag &= ~(CSTOPB | CRTSCTS);
    tio.c_cflag |= CLOCAL | CREAD;

    cfsetispeed(&tio, baud);
    cfsetospeed(&tio, baud);

    if (tcsetattr(fd, TCSANOW, &tio) != 0) {
        return -1;
    }

    tcflush(fd, TCIOFLUSH);
    return 0;
}

void run_echo(int fd) {
    char line[LINE_MAX_LEN + 1];
    size_t len = 0;

    while (1) {
        char c;
        ssize_t n = read(fd, &c, 1);

        if (n < 0) {
            if (errno == EINTR) continue;
            perror("Read error");
            break;
        }
        if (n == 0) continue;

        if (c == '\r' || c == '\n') {
            if (len == 0) continue;

            line[len] = '\0';

            printf("Received: %s\n", line);

            swap_case(line);

            char reply[LINE_MAX_LEN + 3];
            int reply_len = snprintf(reply, sizeof(reply), "%s\r\n", line);

            write(fd, reply, reply_len);
            len = 0;
        } else if (len < LINE_MAX_LEN) {
            line[len++] = c;
        } else {
            len = 0;
        }
    }
}

int main(int argc, char **argv) {
    struct termios saved_tio;
    struct sigaction sa;

    if (argc < 2) {
        fprintf(stderr, "To use this, specify the path to the UART port: %s /dev/ttyAMA0\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("Failed to open port");
        return 1;
    }

    if (setup_uart(fd, B115200, &saved_tio) != 0) {
        perror("Failed to configure port");
        close(fd);
        return 1;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_signal;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("UART Echo server running on %s at 115200 baud. Press Ctrl+C to exit.\n", argv[1]);

    run_echo(fd);

    printf("\nRestoring terminal and shutting down...\n");

    tcsetattr(fd, TCSANOW, &saved_tio);
    close(fd);
    return 0;
}
