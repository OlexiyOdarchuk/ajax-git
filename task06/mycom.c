/*
 * mycom - UART ECHO application
 * Reads lines from a serial port, answers with swapped case, and logs them.
 */

#define _DEFAULT_SOURCE

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define LINE_MAX_LEN 256

volatile sig_atomic_t g_stop = 0;
FILE *g_log = NULL;

void format_time(char *buf, size_t size)
{
    time_t now = time(NULL);
    struct tm tm_now;
    localtime_r(&now, &tm_now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", &tm_now);
}

void log_line(const char *text)
{
    if (g_log == NULL)
        return;
    char ts[32];
    format_time(ts, sizeof(ts));
    fprintf(g_log, "[%s] %s\n", ts, text);
    fflush(g_log);
}

void on_signal(int sig)
{
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

int setup_uart(int fd, speed_t baud, struct termios *saved)
{
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

int run_echo(int fd)
{
    char line[LINE_MAX_LEN + 1];
    size_t len = 0;

    while (!g_stop) {
        char c;
        ssize_t n = read(fd, &c, 1);

        if (n < 0) {
            if (errno == EINTR)
                continue;
            log_line("ERROR: Read error from port");
            return -1;
        }
        if (n == 0)
            continue;

        if (c == '\r' || c == '\n') {
            if (len == 0)
                continue;

            line[len] = '\0';

            char log_msg[LINE_MAX_LEN + 32];
            snprintf(log_msg, sizeof(log_msg), "Received: %s", line);
            log_line(log_msg);
            printf("%s\n", log_msg);

            swap_case(line);

            char reply[LINE_MAX_LEN + 3];
            int reply_len = snprintf(reply, sizeof(reply), "%s\r\n", line);

            if (write(fd, reply, reply_len) < 0) {
                log_line("ERROR: Write error to port");
            }
            len = 0;
        } else if (len < LINE_MAX_LEN) {
            line[len++] = c;
        } else {
            log_line("WARNING: Line too long, buffer flushed");
            len = 0;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    struct termios saved_tio;
    struct sigaction sa;
    char log_filename[64];
    char start_msg[128];
    int rc;

    setvbuf(stdout, NULL, _IOLBF, 0);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <device>\n", argv[0]);
        return 1;
    }

    time_t now = time(NULL);
    struct tm tm_start;
    localtime_r(&now, &tm_start);
    strftime(log_filename, sizeof(log_filename), "%Y%m%d_%H%M%S.log",
             &tm_start);

    g_log = fopen(log_filename, "a");
    if (g_log == NULL) {
        perror("Failed to open log file");
        return 1;
    }

    int fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("Failed to open port");
        fclose(g_log);
        return 1;
    }

    if (setup_uart(fd, B115200, &saved_tio) != 0) {
        perror("Failed to configure port");
        close(fd);
        fclose(g_log);
        return 1;
    }

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_signal;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    snprintf(start_msg, sizeof(start_msg), "Started on %s, logging to %s",
             argv[1], log_filename);
    printf("%s\n", start_msg);
    log_line(start_msg);

    rc = run_echo(fd);

    log_line(rc == 0 ? "Server stopped gracefully" : "Server stopped on error");
    printf("\nRestoring terminal and shutting down...\n");

    tcsetattr(fd, TCSANOW, &saved_tio);
    close(fd);
    fclose(g_log);
    return rc == 0 ? 0 : 1;
}
