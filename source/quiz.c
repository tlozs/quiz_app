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

/* TODO: what if freeing NULL */
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
    qa->answer = strdup(tab + 1); /* TODO: do not copy endline */
    return qa;
}

/* TODO: warn empty quiz */
void play_quiz() {
    int i;
    for (i = 0; i < quiz->size; i++) {
        printf("Question %d: %s\n", i + 1, quiz->qas[i]->question);
        printf("Answer: %s\n", quiz->qas[i]->answer);
    }
    return;
}

