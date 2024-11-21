#include <stdio.h>
#include <string.h>
#include "evaluation.h"
#include "comm.h"

#define ANSWER_SIZE 256

time_t start, end, elapsed;
int correct_answers = 0, asked_questions = 0;

int ask_and_correct_question(QAPair *qa) {
    char user_answer[ANSWER_SIZE];
    char* question;
    char* correct_answer;
    int exit = 0;

    if (gamemode == ONEROUNDER_REVERSED || gamemode == INFINITE_REVERSED) {
        question = qa->answer;
        correct_answer = qa->question;
    }
    else {
        question = qa->question;
        correct_answer = qa->answer;
    }

    asked_questions++;

    print_message(INFO, "%s ", question);
    fgets(user_answer, sizeof(user_answer), stdin);

    /* Remove newline character if present */
    user_answer[strcspn(user_answer, "\n")] = '\0';

    if (strcmp(user_answer, "!exit") == 0)
        exit = 1;
    else if (strcmp(user_answer, correct_answer) == 0)
        correct_answers++;
    else
        print_message(INCORRECT, "%s", correct_answer);

    print_message(INFO, "");
    
    return exit;
}

void start_timer() {
    start = time(NULL);

    return;
}

void stop_timer() {
    end = time(NULL);
    elapsed = end - start;

    return;
}

void evaluate_quiz() {
    double percentage = (double)correct_answers / asked_questions * 100;

    print_message(INFO, "Quiz completed in %ld seconds.", elapsed);
    print_message(INFO, "You got %d out of %d questions correct.", correct_answers, asked_questions);
    print_message(INFO, "That's %.2f%%!", percentage);
}