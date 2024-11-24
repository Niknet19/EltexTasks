#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// Операции P (-1) и V (+1)
struct sembuf p_op = {0, -1, 0};
struct sembuf v_op = {0, 1, 0};

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

int child_process(int pipefd[2], int semid) {
    close(pipefd[0]);
    int r = 0;

    while (1) {
        if (semop(semid, &p_op, 1) == -1) {
            perror("Ошибка P-операции в дочернем процессе");
            break;
        }

        read_from_file("numbers.bin");

        if (semop(semid, &v_op, 1) == -1) {
            perror("Ошибка V-операции в дочернем процессе");
            break;
        }

        r = rand() % 100;
        if (write(pipefd[1], &r, sizeof(int)) == -1) {
            perror("Ошибка записи в pipe");
            break;
        }
        fprintf(stdout, "Sent number: %d\n", r);

        sleep(1);
    }

    close(pipefd[1]);
    return 0;
}

int parent_process(int num_of_numbers, int pipefd[2], int semid, pid_t child_pid) {
    close(pipefd[1]);
    int r = 0;

    for (size_t i = 0; i < num_of_numbers; i++) {
        if (read(pipefd[0], &r, sizeof(int)) == -1) {
            perror("Ошибка чтения из pipe");
            break;
        }

        // Блокируем доступ к файлу
        if (semop(semid, &p_op, 1) == -1) {
            perror("Ошибка P-операции в родительском процессе");
            break;
        }

        fprintf(stdout, "Received number: %d\n", r);
        write_to_file("numbers.bin", &r);

        if (semop(semid, &v_op, 1) == -1) {
            perror("Ошибка V-операции в родительском процессе");
            break;
        }
    }

    close(pipefd[0]);
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
    key_t key = ftok("aaaaa", 2);
    // Создаем семафор
    int semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Ошибка создания семафора");
        return EXIT_FAILURE;
    }

    // Инициализируем семафор 1
    if (semctl(semid, 0, SETVAL, 1) == -1) {
        perror("Ошибка инициализации семафора");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
        return EXIT_FAILURE;
    }

    if (pid > 0) {
        // Родительский процесс
        parent_process(num_of_args, pipefd, semid, pid);
        waitpid(pid, NULL, 0);

        // Удаляем семафор
        if (semctl(semid, 0, IPC_RMID) == -1) {
            perror("Ошибка удаления семафора");
        }
    } else if (pid == 0) {
        // Дочерний процесс
        // fprintf(stdout, "child: %d\n", getpid());
        child_process(pipefd, semid);
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}
