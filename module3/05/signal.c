#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

volatile sig_atomic_t file_access_allowed = 1;

// Обработчик сигналов для дочернего процесса
void handle_signal(int signal) {
    if (signal == SIGUSR1) {
        file_access_allowed = 0;  // Блокировка доступа
    } else if (signal == SIGUSR2) {
        file_access_allowed = 1;  // Разрешение доступа
    }
}

int write_to_file(char* path, int* num) {
    int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("Не удалось открыть файл");
        return -1;
    }
    if (write(fd, num, sizeof(int)) == -1) {
        perror("Ошибка записи");
    }
    if (close(fd) == -1) {
        perror("Не удалось закрыть файл");
        return -1;
    }
    return 0;
}

void read_from_file(char* path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Не удалось открыть файл");
        return;
    }
    int num;
    printf("Содержимое файла:\n");
    while (read(fd, &num, sizeof(int)) > 0) {
        printf("%d ", num);
    }
    printf("\n");
    if (close(fd) == -1) {
        perror("Не удалось закрыть файл");
    }
}

int child_process(int pipefd[2]) {
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);

    close(pipefd[0]);  // Закрыть конец для чтения
    int r = 0;

    while (1) {
        // Ждём разрешение доступа к файлу
        while (!file_access_allowed) {
            pause();
        }

        read_from_file("numbers.bin");

        // Генерация нового числа
        r = rand() % 100;
        if (write(pipefd[1], &r, sizeof(int)) == -1) {
            perror("Ошибка записи в pipe");
            break;
        }
        fprintf(stdout, "Sent number: %d\n", r);
        // sleep(3);
    }

    close(pipefd[1]);  // Закрыть конец для записи
    return 0;
}

int parent_process(int num_of_numbers, int pipefd[2], pid_t child_pid) {
    close(pipefd[1]);  // Закрыть конец для записи
    int r = 0;

    for (size_t i = 0; i < num_of_numbers; i++) {
        if (read(pipefd[0], &r, sizeof(int)) == -1) {
            perror("Ошибка чтения из pipe");
            break;
        }

        // Блокируем доступ к файлу
        kill(child_pid, SIGUSR1);

        fprintf(stdout, "Received number: %d\n", r);
        write_to_file("numbers.bin", &r);

        // Разрешаем доступ к файлу
        kill(child_pid, SIGUSR2);
    }

    close(pipefd[0]);  // Закрыть конец для чтения
    return 0;
}

int main(int argc, char** argv) {
    srand(time(NULL));

    if (argc < 2) {
        fprintf(stderr, "Недостаточно аргументов.\n");
        return EXIT_FAILURE;
    }
    int num_of_args = atoi(argv[1]);
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("Ошибка pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
        return EXIT_FAILURE;
    }

    if (pid > 0) {
        // Родительский процесс
        parent_process(num_of_args, pipefd, pid);
        waitpid(pid, NULL, 0);
    } else if (pid == 0) {
        // Дочерний процесс
        fprintf(stdout, "child: %d\n", getpid());
        child_process(pipefd);
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}
