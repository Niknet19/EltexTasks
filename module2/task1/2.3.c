#include <stdarg.h>
#include <stdio.h>

#define OPERATION_NUMBER 4

typedef double (*operation_func_ptr)(size_t, ...);
double sum(size_t num_args, ...) {
    double result = 0.0;
    va_list args;
    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i++) {
        result += va_arg(args, double);
    }
    va_end(args);
    return result;
}

double sub(size_t num_args, ...) {
    double result = 0;
    va_list args;
    va_start(args, num_args);
    result = va_arg(args, double);
    for (size_t i = 1; i < num_args; i++) {
        // result += (i == 0) ? va_arg(args, double) : -(va_arg(args, double));
        result -= va_arg(args, double);
    }
    va_end(args);
    return result;
}

double mul(size_t num_args, ...) {
    double result = 1.0;
    va_list args;
    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i++) {
        result *= va_arg(args, double);
    }
    va_end(args);
    return result;
}

double div(size_t num_args, ...) {
    double result = 0.0;
    va_list args;
    va_start(args, num_args);
    result = va_arg(args, double);
    for (size_t i = 1; i < num_args; i++) {
        double divider = va_arg(args, double);
        if (divider == 0) {
            fprintf(stderr, "Error: Zero division.\n");
            return 0;
        }
        result = result / divider;
    }
    va_end(args);
    return result;
}

void get_user_input(size_t* num_args, double* args) {
    printf("Введите количество аргументов (2-4): ");
    scanf("%ld", num_args);

    if (*num_args <= 1 || *num_args > 4) {
        printf("Количество аргументов должно быть от 2 до 4.\n");
        *num_args = 0;
        return;
    }

    printf("Введите %ld аргумента(ов):\n", *num_args);
    for (size_t i = 0; i < *num_args; i++) {
        printf("Аргумент %ld: ", i + 1);
        while (scanf("%lf", &args[i]) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите число: ");
            while (getchar() != '\n')
                ;  // Очищаем буфер
        }
    }
}

typedef struct Operation {
    const char* op_name;
    operation_func_ptr func;
} Operation;

Operation operations[] = {{"Сложение", sum}, {"Вычитание", sub}, {"Умножение", mul}, {"Деление", div}};

int main() {
    // printf("Результат: %lf\n", operations[op_index - 1].func(num_args, args[0]));
    int choice;
    double args[4];
    size_t num_args;

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
                ;
        }

        if (choice == 5) {
            break;
        }

        get_user_input(&num_args, args);
        if (num_args == 0) continue;  // Если введено некорректное количество аргументов

        double result;

        result = operations[choice - 1].func(num_args, args[0], args[1], args[2], args[3]);
        printf("Результат операции %s : %lf\n", operations[choice - 1].op_name, result);
    }

    return 0;
}