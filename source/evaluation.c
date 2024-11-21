#include <stdio.h>
#include <string.h>
#include "evaluation.h"
#include "comm.h"

#define ANSWER_SIZE 256

time_t start, end, elapsed;
int correct_answers = 0;

void ask_and_correct_question(QAPair *qa) {
    char answer[ANSWER_SIZE];

    print_message(INFO, "%s ", qa->question);
    fgets(answer, sizeof(answer), stdin);

    /* Remove newline character if present */
    answer[strcspn(answer, "\n")] = '\0';

    if (strcmp(answer, qa->answer) == 0)
        correct_answers++;
    else
        print_message(INCORRECT, "%s", qa->answer);

    print_message(INFO, "");
    
    return;
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
    double percentage = (double)correct_answers / quiz->size * 100;

    print_message(INFO, "Quiz completed in %ld seconds.", elapsed);
    print_message(INFO, "You got %d out of %d questions correct.", correct_answers, quiz->size);
    print_message(INFO, "That's %.2f%%!", percentage);
}