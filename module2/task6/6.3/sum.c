#include "calc.h"
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