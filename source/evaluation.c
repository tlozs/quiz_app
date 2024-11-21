#include <stdio.h>
#include <string.h>
#include "evaluation.h"
#include "comm.h"

#define ANSWER_SIZE 256

time_t start, end, elapsed;

void ask_and_correct_question(QAPair *qa) {
    char answer[ANSWER_SIZE];

    print_message(INFO, "\n%s ", qa->question);
    fgets(answer, sizeof(answer), stdin);

    /* Remove newline character if present */
    answer[strcspn(answer, "\n")] = '\0';

    if (strcmp(answer, qa->answer) != 0)
        print_message(INCORRECT, "%s", qa->answer);
    
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
    print_message(INFO, "\nQuiz completed in %ld seconds.", elapsed);
}