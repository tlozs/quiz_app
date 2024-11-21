#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"
#include "comm.h"

Quiz *quiz;

void init_quiz() {
    quiz = malloc(sizeof(Quiz));
    if (quiz == NULL)
        print_message(FATAL, "Memory allocation failed.");
    quiz->size = 0;
    quiz->capacity = 16;
    quiz->qas = malloc(quiz->capacity * sizeof(QAPair*));
    if(quiz->qas == NULL)
        print_message(FATAL, "Memory allocation failed.");
    return;
}

void extend_quiz(QAPair *qa) {
    if (quiz->size == quiz->capacity) {
        quiz->capacity *= 2;
        quiz->qas = realloc(quiz->qas, quiz->capacity * sizeof(QAPair*));
        if(quiz->qas == NULL)
            print_message(FATAL, "Memory allocation failed.");
    }
    quiz->qas[quiz->size++] = qa;
    return;
}

void shrink_quiz_size() {
    quiz->capacity = quiz->size;
    quiz->qas = realloc(quiz->qas, quiz->capacity * sizeof(QAPair*));
    return;
}

void free_quiz() {
    int i;
    for (i = 0; i < quiz->size; i++) {
        free(quiz->qas[i]->question);
        free(quiz->qas[i]->answer);
        free(quiz->qas[i]);
    }
    free(quiz->qas);
    free(quiz);
    return;
}

QAPair* parse_to_qa(const char *line) {
    QAPair *qa = malloc(sizeof(QAPair));
    char *tab = strchr(line, '\t');
    if (qa == NULL)
        print_message(FATAL, "Memory allocation failed.");
    
    qa->question = malloc(tab - line + 1);
    if (qa->question == NULL)
        print_message(FATAL, "Memory allocation failed.");
    strncpy(qa->question, line, tab - line);
    qa->question[tab - line] = '\0';

    /* get rid of newline at the end:
       no space for null terminator, the newline character will be replaced by it */
       /* TODO: check and only swap if there is newline */
    qa->answer = malloc(strlen(tab + 1));
    if (qa->answer == NULL)
        print_message(FATAL, "Memory allocation failed.");
    qa->answer = strcpy(qa->answer, tab + 1);
    qa->answer[strlen(tab + 1)-1] = '\0';
    return qa;
}

void play_quiz() {
    int i;

    if (quiz->size == 0)
        print_message(FATAL, "Could not read any data from the specified inputs.");

    for (i = 0; i < quiz->size; i++) {
        printf("Question %d: %s\n", i + 1, quiz->qas[i]->question);
        printf("Answer: %s\n\n", quiz->qas[i]->answer);
    }
    return;
}

