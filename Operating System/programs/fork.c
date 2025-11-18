#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    int fd[2];
    pid_t pid;
    char write_msg[] = "hello from parent";
    char read_msg[sizeof write_msg];

    if (pipe(fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        /* Child process: read from the pipe and report what we got */
        close(fd[1]);
        if (read(fd[0], read_msg, sizeof read_msg) == -1) {
            perror("read");
            close(fd[0]);
            return EXIT_FAILURE;
        }
        printf("child received : %s\n", read_msg);
        close(fd[0]);
    } else {
        /* Parent process: write the message into the pipe */
        close(fd[0]);
        if (write(fd[1], write_msg, strlen(write_msg) + 1) == -1) {
            perror("write");
            close(fd[1]);
            return EXIT_FAILURE;
        }
        close(fd[1]);
    }
    return EXIT_SUCCESS;
}