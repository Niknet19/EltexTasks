#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int write_to_file(char* path, int* num) {
    int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("Не удалось открыть файл");
        return -1;
    }
    if (write(fd, num, sizeof(int)) == -1) {
        perror("Ошибка записи");
    };
    if (close(fd) == -1) {
        perror("Не удалось закрыть файл");
        return -1;
    }
}

int parent_process(int num_of_numbers, int pipefd[2]) {
    if (close(pipefd[0]) == -1) {
        perror("Не удалось закрыть pipe для чтения");
    }
    int r = 0;
    for (size_t i = 0; i < num_of_numbers; i++) {
        r = rand() % 100;
        if (write(pipefd[1], &r, sizeof(int)) == -1) {
            perror("Ошибка записи в pipe");
            break;
        }
        fprintf(stdout, "Sent number: %d\n", r);
        sleep(1);
    }
    if (close(pipefd[1]) == -1) {
        perror("Не удалось закрыть pipe для записи");
    }
}

int child_process(int num_of_numbers, int pipefd[2]) {
    if (close(pipefd[1]) == -1) {
        perror("Не удалось закрыть pipe для записи");
    }
    int r = 0;
    for (size_t i = 0; i < num_of_numbers; i++) {
        if (read(pipefd[0], &r, sizeof(int)) == -1) {
            perror("Ошибка чтения из в pipe");
            break;
        }
        fprintf(stdout, "Received number: %d\n", r);
        write_to_file("numbers.bin", &r);
    }
    if (close(pipefd[0]) == -1) {
        perror("Не удалось закрыть pipe для чтения");
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));
    if (argc < 2) {
        fprintf(stderr, "Недостаточно аргументов.\n", argv[0]);
        return EXIT_FAILURE;
    }
    int num_of_args = atoi(argv[1]);
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Ошибка pipe");
    }
    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
    }
    // pause();
    if (pid > 0) {
        //Родительский процесс
        parent_process(num_of_args, pipefd);
        waitpid(pid, NULL, 0);
    } else if (pid == 0) {
        //Дочерний процесс
        child_process(num_of_args, pipefd);
        exit(EXIT_SUCCESS);
    }
    return EXIT_SUCCESS;
}