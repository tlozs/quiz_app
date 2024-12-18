#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <conio.h>
#include "comm.h"
#include "quiz.h"

void welcome_toast(int count) {
    print_message(INFO, "\nWelcome to the quiz!");
    print_message(INFO, "You will need to answer a pool of %d questions asked in random order.", count);
    print_message(INFO, "Your job is to type your answer and then press Enter.");
    print_message(INFO, "If you get the answer wrong, the correct answer will be shown.");
    print_message(INFO, "Type '!exit' to quit the quiz at any time.");
    print_message(INFO, "Good luck!\n");

    return;
}

void gamemode_select() {
    int answer_correct = 0;

    while (!answer_correct)
    {
        print_message(INFO, "Select the game mode:");
        print_message(INFO, "  1. One Rounder (Every question is asked once)");
        print_message(INFO, "  2. One Rounder Reversed (Answers are questions, and vice versa)");
        print_message(INFO, "  3. Infinite (Questions are asked until you quit)");
        print_message(INFO, "  4. Infinite Reversed (Answers are questions, and vice versa)\n");

        answer_correct = 1;

        switch (getch()) {
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
                answer_correct = 0;
                break;
        }
    }
    
    printf("Selected game mode: ");
    switch (gamemode)
    {
        case ONEROUNDER:
            print_message(INFO, "One Rounder");
            break;
        case ONEROUNDER_REVERSED:
            print_message(INFO, "One Rounder Reversed");
            break;
        case INFINITE:
            print_message(INFO, "Infinite");
            break;
        case INFINITE_REVERSED:
            print_message(INFO, "Infinite Reversed");
            break;
    }
    print_message(INFO, "");

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

int getchar_equals(char c) {
    char input = tolower(getch());

    if (c == 0)
        printf("\n");
    else
        printf("%c\n", input);

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