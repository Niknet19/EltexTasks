#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
Можно добавить перенаправление вывода в файл
и перенаправление вывода от одного процесса к другому
*/

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

void execute_command(char *command) {
    char *args[MAX_ARG_SIZE];
    char *token;
    int i = 0;

    token = strtok(command, " \n");
    while (token != NULL && i < MAX_ARG_SIZE - 1) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;  // Последний аргумент должен быть NULL для exec

    pid_t pid = fork();

    if (pid < 0) {
        perror("Не удалось создать процесс.\n");
        return;
    }

    if (pid == 0) {
        // Дочерний процесс
        if (execvp(args[0], args) == -1) {
            perror("Ошибка exec");
            exit(EXIT_FAILURE);
        }
    } else {
        // Родительский процесс
        waitpid(pid, NULL, 0);
    }
}

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("Введите команду (или 'exit' для выхода): ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("Ошибка чтения ввода");
            continue;
        }

        if (strcmp(input, "exit\n") == 0) {
            break;
        }

        execute_command(input);
    }

    return EXIT_SUCCESS;
}
