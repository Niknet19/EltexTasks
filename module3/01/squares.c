#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_ARG_SIZE 50

inline float calculate_area(float side) { return side * side; }

void print_squares(size_t startindex, size_t endindex, char** args) {
    for (size_t i = startindex; i <= endindex; i++) {
        fprintf(stdout, "PID: %d : Площадь: %.3f\n", getpid(), calculate_area(atof(args[i])));
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s сторона1 сторона2 ...\n", argv[0]);
        return 1;
    }
    float args[MAX_ARG_SIZE];

    int num_sides = argc - 1;
    int mid = num_sides / 2;
    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
    }

    if (pid > 0) {
        //Родительский процесс
        print_squares(1, mid, argv);
        wait(NULL);
    } else if (pid == 0) {
        //Дочерний процесс
        print_squares(mid + 1, argc, argv);
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}