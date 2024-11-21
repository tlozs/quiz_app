#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"
#include "comm.h"
#include "evaluation.h"

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
    if(quiz->qas == NULL)
        print_message(FATAL, "Memory allocation failed.");

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
    char *tab = strchr(line, '\t');
    QAPair *qa = malloc(sizeof(QAPair));
    
    if (qa == NULL)
        print_message(FATAL, "Memory allocation failed.");
    
    parse_question(qa, line, tab);
    parse_answer(qa, tab);
    
    return qa;
}


void parse_question(QAPair *qa, const char *line, char *tab) {
    qa->question = malloc(tab - line + 1);
    if (qa->question == NULL)
        print_message(FATAL, "Memory allocation failed.");
    
    /* since source is longer, no null terminator is copied, so we add it manually */
    strncpy(qa->question, line, tab - line);
    qa->question[tab - line] = '\0';

    return;
}

void parse_answer(QAPair *qa, char *tab) {
    /* Remove newline character if present (from the end of the line)*/
    tab[strcspn(tab, "\n")] = '\0';

    qa->answer = malloc(strlen(tab + 1) + 1);
    if (qa->answer == NULL)
        print_message(FATAL, "Memory allocation failed.");
    
    /* This copies the null terminator as well */
    strcpy(qa->answer, tab + 1);
    
    return;
}

QAPair *random_question(int range, int *out_index) {
    *out_index = rand() % range;

    return quiz->qas[*out_index];
}

void swap_qas(int i, int j) {
    QAPair *temp = quiz->qas[i];
    quiz->qas[i] = quiz->qas[j];
    quiz->qas[j] = temp;

    return;
}

void play_quiz() {
    int current;
    int range = quiz->size;
    if (range == 0)
        print_message(FATAL, "Could not read any data from the specified inputs.");

    welcome_toast(range);
    print_message(QUESTION, "Press any key to start the quiz...");
    getchar_equals(0);

    /* clear any previous error messages */
    clear_screen();
    welcome_toast(range);

    start_timer();

    while (range) {
        QAPair *qa = random_question(range, &current);
        ask_and_correct_question(qa);
        swap_qas(current, --range);
    }
    
    stop_timer();

    return;
}

