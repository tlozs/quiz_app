#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <conio.h>
#include "comm.h"
#include "quiz.h"

void welcome_toast(int count) {
    print_message(INFO, "\nWelcome to the quiz!");
    print_message(INFO, "You will be asked %d questions in random order.", count);
    print_message(INFO, "Your job is to type your answer and then press Enter.");
    print_message(INFO, "If you get the answer wrong, the correct answer will be shown.");
    print_message(INFO, "Good luck!");

    return;
}

void gamemode_select() {
    int characters_skipped;
    int answer;
    print_message(INFO, "Select the game mode:");
    print_message(INFO, "1. One Rounder (Every question is asked once)");
    print_message(INFO, "2. One Rounder Reversed (Answers are questions, and vice versa)");
    print_message(INFO, "3. Infinite (Questions are asked until you quit)");
    print_message(INFO, "4. Infinite Reversed (Answers are questions, and vice versa)");

    /* Force switch to the default case if multiple characters are entered */
    answer = get_one_char(&characters_skipped);
    if (characters_skipped)
        answer = 0;

    switch (answer) {
        case '1':
            gamemode = ONEROUNDER;
            break;
        case '2':
            gamemode = ONEROUNDER_REVERSED;
            break;
        case '3':
            gamemode = INFINITE;
            break;
        case '4':
            gamemode = INFINITE_REVERSED;
            break;
        default:
            clear_screen();
            print_message(ERROR, "Invalid game mode selected.");
            welcome_toast(quiz->size);
            gamemode_select();
            break;
    }

    return;
}

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
        case INCORRECT:
            color_code = "\033[1;31m";
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
            color_code = "\033[1;31m";
            prefix = "Fatal error: ";
            break;
    }
    
    printf("%s%s", color_code, prefix);
    vprintf(message, args);
    va_end(args);
    printf("\033[0m");

    printf(type == QUESTION ? "" : "\n");

    if (type == FATAL) {
        printf("\n");
        free_quiz();
        exit(1);
    }

    return;
}

int get_one_char(int *out_skipped) {
    int input = tolower(getchar());
    if (out_skipped) *out_skipped = 0;

    while (!(input == '\n' || getchar() == '\n'))
        if (out_skipped) (*out_skipped)++;

    return input;
}

int getchar_equals(char c) {
    char input = 0;

    if (c == 0)  {
        getch();
        printf("\n");
    }
    else
        input = get_one_char(NULL);
    
    return input == c;
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    return;
}