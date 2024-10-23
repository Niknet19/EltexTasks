#include <stdarg.h>
#include <stdio.h>

#define OPERATION_NUMBER 4

typedef double (*operation_func_ptr)(int, ...);
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

typedef struct Operation {
    const char* op_name;
    operation_func_ptr func;
} Operation;

Operation operations[] = {{"Сложение", sum},
                          {"Вычитание", sub},
                          {"Умножение", mul},
                          {"Деление", div}};

int main() {
    int op_index = 0;
    int num_args = 1;
    while (1) {
        printf("Введите номер операции:\n");
        for (size_t i = 0; i < OPERATION_NUMBER; i++) {
            printf("%d : %s\n", i + 1, operations[i].op_name);
        }
        printf(">> ");
        scanf("%d", &op_index);

        printf("\nВведите количество чисел:");
        scanf("%d", &num_args);

        printf("\nВведите аргументы:");
        double args[num_args];
        for (size_t i = 0; i < num_args; i++) {
            scanf("%lf", &args[i]);
        }

        switch (num_args) {
            case 1:
                printf("Результат: %lf\n",
                       operations[op_index - 1].func(num_args, args[0]));
                break;
            case 2:
                printf("Результат: %lf\n", operations[op_index - 1].func(
                                               num_args, args[0], args[1]));
                break;
            case 3:
                printf("Результат: %lf\n",
                       operations[op_index - 1].func(num_args, args[0], args[1],
                                                     args[2]));
            case 4:
                printf("Результат: %lf\n",
                       operations[op_index - 1].func(num_args, args[0], args[1],
                                                     args[2], args[3]));
                break;
            default:
                break;
        }
    }

    return 0;
}