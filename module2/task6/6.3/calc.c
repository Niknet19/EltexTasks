
#include "calc.h"
double sum(size_t num_args, ...) {
    double result = 0.0;
    va_list args;
    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i++) {
        result += va_arg(args, double) + 1000;
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