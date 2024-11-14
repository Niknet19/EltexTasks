#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef double (*operation_func_ptr)(size_t, ...);

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
                ;
        }
    }
}

typedef struct Operation {
    const char* op_name;
    operation_func_ptr func;
    void* lib_handle;
} Operation;

int main() {
    // Загружаем каждую библиотеку отдельно и получаем адрес функции
    Operation operations[] = {{"Сложение", NULL, dlopen("./libsum.so", RTLD_LAZY)},
                              {"Вычитание", NULL, dlopen("./libsub.so", RTLD_LAZY)},
                              {"Умножение", NULL, dlopen("./libmul.so", RTLD_LAZY)},
                              {"Деление", NULL, dlopen("./libdiv.so", RTLD_LAZY)}};

    // Проверяем, что все библиотеки загружены успешно
    for (int i = 0; i < 4; i++) {
        if (!operations[i].lib_handle) {
            fprintf(stderr, "Ошибка загрузки библиотеки %s: %s\n", operations[i].op_name, dlerror());
            // Освобождаем все успешно загруженные библиотеки перед завершением
            for (int j = 0; j < i; j++) {
                dlclose(operations[j].lib_handle);
            }
            return 1;
        }
    }

    // Получаем функции из каждой загруженной библиотеки
    operations[0].func = (operation_func_ptr)dlsym(operations[0].lib_handle, "sum");
    operations[1].func = (operation_func_ptr)dlsym(operations[1].lib_handle, "sub");
    operations[2].func = (operation_func_ptr)dlsym(operations[2].lib_handle, "mul");
    operations[3].func = (operation_func_ptr)dlsym(operations[3].lib_handle, "div");

    // Проверка на ошибки загрузки функций
    const char* error;
    for (int i = 0; i < 4; i++) {
        if ((error = dlerror()) != NULL) {
            fprintf(stderr, "Ошибка при загрузке функции %s: %s\n", operations[i].op_name, error);
            for (int j = 0; j < 4; j++) {
                dlclose(operations[j].lib_handle);
            }
            return 1;
        }
    }

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
        if (num_args == 0) continue;

        double result = operations[choice - 1].func(num_args, args[0], args[1], args[2], args[3]);
        printf("Результат операции %s : %lf\n", operations[choice - 1].op_name, result);
    }

    // Закрываем все загруженные библиотеки
    for (int i = 0; i < 4; i++) {
        dlclose(operations[i].lib_handle);
    }

    return 0;
}
