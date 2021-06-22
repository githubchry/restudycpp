
#include <stdarg.h>
#include <stdio.h>


void printf_wrapper(const char *format, ...) {
    va_list marker;
    va_start(marker, format);
    vprintf(format, marker);
    va_end(marker);
}

int main() {
    printf_wrapper("hello %s\n", "world");
    return 0;
}