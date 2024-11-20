#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include "comm.h"
#include "quiz.h"

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
        case FATAL:
            color_code = "\033[1;35m"; /* TODO: check colors */
            prefix = "Fatal error: ";
            break;
    }
    printf("%s%s", color_code, prefix);
    vprintf(message, args);
    printf("\033[0m");
    printf(type == QUESTION ? "" : "\n");
    va_end(args);
    if (type == FATAL) {
        free_quiz();
        exit(1);
    }
    return;
}

int getchar_equals(char c) {
    char input = tolower(getchar());
    int result = input == c;
    while (!(input == '\n' || getchar() == '\n'));
    return result;
}