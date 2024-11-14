#include "calc.h"
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