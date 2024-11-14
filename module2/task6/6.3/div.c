#include "calc.h"
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