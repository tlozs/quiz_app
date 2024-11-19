#include <stdio.h>
#include <ctype.h>
#include "comm.h"
#include <stdarg.h>

void print_message(message_type type, const char *message, ...) {
    char *color_code;
    char *prefix = "";
    va_list args;
    va_start(args, message);
    switch (type) {
        case INFO:
        case QUESTION:
            color_code = "\033[1;32m";
            prefix = "";
            break;
        case WARNING:
            color_code = "\033[1;33m";
            prefix = "Warning: ";
            break;
        case ERROR:
            color_code = "\033[1;31m";
            prefix = "Error: ";
            break;
    }
    printf("%s%s", color_code, prefix);
    vprintf(message, args);
    printf("\033[0m");
    printf(type == QUESTION ? "" : "\n");
    va_end(args);
}

int getchar_equals(char c) {
    char input = tolower(getchar());
    int result = input == c;
    while (!(input == '\n' || getchar() == '\n'));
    return result;
}