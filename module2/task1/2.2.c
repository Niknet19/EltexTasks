#include <stdarg.h>
#include <stdio.h>

double sum(int num_args, ...) {
    double result = 0.0;
    va_list args;
    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i++) {
        result += va_arg(args, double);
    }
    va_end(args);
    return result;
}

double sub(int num_args, ...) {
    double result = 0;
    va_list args;
    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i++) {
        result += (i == 0) ? va_arg(args, double) : -(va_arg(args, double));
        // result *= va_arg(args, double);
    }
    va_end(args);
    return result;
}

double mul(int num_args, ...) {
    double result = 1.0;
    va_list args;
    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i++) {
        result *= va_arg(args, double);
    }
    va_end(args);
    return result;
}

double div(int num_args, ...) {
    double result = 0.0;
    va_list args;
    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i++) {
        if (i == 0)
            result = va_arg(args, double);
        else
            result = result / va_arg(args, double);
    }
    va_end(args);
    return result;
}

void get_user_input(int* num_args, double* args) {
    printf("Введите количество аргументов (2-4): ");
    scanf("%d", num_args);

    if (*num_args <= 1 || *num_args > 4) {
        printf("Количество аргументов должно быть от 2 до 4.\n");
        *num_args = 0;
        return;
    }

    printf("Введите %d аргумента(ов):\n", *num_args);
    for (int i = 0; i < *num_args; i++) {
        printf("Аргумент %d: ", i + 1);
        while (scanf("%lf", &args[i]) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите число: ");
            while (getchar() != '\n')
                ;  // Очищаем буфер
        }
    }
}

int main() {
    int choice;
    double args[4];  // Максимум 4 аргумента
    int num_args;

    while (1) {
        printf("\nВыберите действие:\n");
        printf("1. Сложение\n");
        printf("2. Вычитание\n");
        printf("3. Умножение\n");
        printf("4. Деление\n");
        printf("5. Выход\n");
        printf("Ваш выбор: ");

        while (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("Ошибка ввода. Пожалуйста, введите число от 1 до 5: ");
            while (getchar() != '\n')
                ;  // Очищаем буфер
        }

        if (choice == 5) {
            break;  // Выход из программы
        }

        get_user_input(&num_args, args);
        if (num_args == 0) continue;  // Если введено некорректное количество аргументов

        double result;
        switch (choice) {
            case 1:
                result = sum(num_args, args[0], args[1], args[2], args[3]);
                printf("Результат сложения: %lf\n", result);
                break;
            case 2:
                result = sub(num_args, args[0], args[1], args[2], args[3]);
                printf("Результат вычитания: %lf\n", result);
                break;
            case 3:
                result = mul(num_args, args[0], args[1], args[2], args[3]);
                printf("Результат умножения: %lf\n", result);
                break;
            case 4:
                result = div(num_args, args[0], args[1], args[2], args[3]);
                if (result != 0) {
                    printf("Результат деления: %lf\n", result);
                }
                break;
        }
    }

    return 0;
}