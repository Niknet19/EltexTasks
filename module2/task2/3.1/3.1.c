#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void print_permissions(mode_t mode) {
    char permissions[10] = "---------";

    permissions[0] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[1] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[2] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[3] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[4] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[5] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[6] = (mode & S_IROTH) ? 'r' : '-';
    permissions[7] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[8] = (mode & S_IXOTH) ? 'x' : '-';

    printf("Буквенное представление: %s\n", permissions);
    printf("Цифровое представление: %o\n", mode & 0777);
    // printf("Цифровое представление: %o\n", mode);
    printf("Битовое представление: ");
    for (int i = 8; i >= 0; i--) {
        printf("%d", (mode >> i) & 1);
    }
    printf("\n");
}

mode_t parse_numeric_permissions(const char *input) {
    mode_t mode = 0;
    if (strlen(input) == 3 && input[0] >= '0' && input[0] <= '7' && input[1] >= '0' && input[1] <= '7' &&
        input[2] >= '0' && input[2] <= '7') {
        (mode |= (input[0] - '0'));
        mode <<= 3;
        (mode |= (input[1] - '0'));
        mode <<= 3;
        (mode |= (input[2] - '0'));
        // return (input[0] - '0') * 64 + (input[1] - '0') * 8 + (input[2] - '0');
        return mode;
    } else {
        fprintf(stderr, "Неверный формат цифровых прав доступа.\n");
        exit(EXIT_FAILURE);
    }
}

mode_t parse_symbolic_permissions(const char *input) {
    if (strlen(input) != 9) {
        fprintf(stderr, "Неверный формат буквенных прав доступа.\n");
        exit(EXIT_FAILURE);
    }

    mode_t mode = 0;

    // Проверка каждого символа на наличие прав для пользователя, группы и остальных
    if (input[0] == 'r') mode |= S_IRUSR;
    if (input[1] == 'w') mode |= S_IWUSR;
    if (input[2] == 'x') mode |= S_IXUSR;
    if (input[3] == 'r') mode |= S_IRGRP;
    if (input[4] == 'w') mode |= S_IWGRP;
    if (input[5] == 'x') mode |= S_IXGRP;
    if (input[6] == 'r') mode |= S_IROTH;
    if (input[7] == 'w') mode |= S_IWOTH;
    if (input[8] == 'x') mode |= S_IXOTH;

    return mode;
}

void modify_permissions(mode_t *mode, const char *command) {
    char who = command[0];
    char op = command[1];
    char perm = command[2];
    mode_t bit = 0;

    // Определяем биты, которые нужно изменить
    switch (perm) {
        case 'r':
            bit = S_IRUSR | S_IRGRP | S_IROTH;
            break;
        case 'w':
            bit = S_IWUSR | S_IWGRP | S_IWOTH;
            break;
        case 'x':
            bit = S_IXUSR | S_IXGRP | S_IXOTH;
            break;
        default:
            fprintf(stderr, "Неизвестная команда модификации '%c'.\n", perm);
            return;
    }
    if (strchr("augo", who) == NULL) {
        fprintf(stderr, "Неверный символ для прав '%c'. Используйте u, g или o.\n", who);
        return;
    }
    if (who == 'u') bit &= S_IRWXU;
    if (who == 'g') bit &= S_IRWXG;
    if (who == 'o') bit &= S_IRWXO;
    if (who == 'a') bit &= (S_IRWXU | S_IRWXG | S_IRWXO);
    // Применяем операцию добавления или удаления
    if (op == '+')
        *mode |= bit;
    else if (op == '-')
        *mode &= ~bit;
    else {
        fprintf(stderr, "Неверный оператор '%c'. Используйте + или -.\n", op);
        return;
    }

    printf("Новые права доступа:\n");
    print_permissions(*mode);
}

void get_file_permissions(const char *filename, mode_t *mode) {
    struct stat file_stat;
    if (stat(filename, &file_stat) < 0) {
        perror("Ошибка при получении информации о файле");
        return;
    }
    *mode = file_stat.st_mode;
    printf("Права доступа для файла '%s':\n", filename);
    print_permissions(*mode);
}

int main() {
    char filename[256];
    char input[10];
    char command[10];
    mode_t mode = 0;
    int choice;
    int file_loaded = 0;

    while (1) {
        printf("\nВыберите действие:\n");
        printf("1. Ввести права доступа и вывести их битовое представление\n");
        printf("2. Ввести имя файла и вывести права доступа\n");
        printf("3. Модифицировать права доступа\n");
        printf("0. Выйти\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите права доступа: ");
                scanf("%s", input);

                if (strspn(input, "01234567") == 3) {
                    mode = parse_numeric_permissions(input);
                } else {
                    mode = parse_symbolic_permissions(input);
                }

                print_permissions(mode);
                file_loaded = 1;
                break;

            case 2:
                printf("Введите имя файла: ");
                scanf("%s", filename);
                get_file_permissions(filename, &mode);
                file_loaded = 1;
                break;

            case 3:
                if (!file_loaded) {
                    printf("Сначала введите права доступа (пункт 1) или имя файла (пункт 2).\n");
                    break;
                }
                printf("Введите команду для модификации прав доступа: ");
                scanf("%s", command);
                modify_permissions(&mode, command);
                break;

            case 0:
                printf("Выход из программы.\n");
                exit(0);

            default:
                printf("Неверный выбор. Повторите попытку.\n");
                break;
        }
    }
    return 0;
}
