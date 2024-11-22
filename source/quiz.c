#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"
#include "comm.h"
#include "fs_utils.h"
#include "evaluation.h"

Quiz *quiz;
GameMode gamemode;

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

int extend_quiz(QAPair *qa) {
    int success = 0;
    
    if (qa != NULL) {
        if (quiz->size == quiz->capacity) {
            quiz->capacity *= 2;
            
            quiz->qas = realloc(quiz->qas, quiz->capacity * sizeof(QAPair*));
            if(quiz->qas == NULL)
                print_message(FATAL, "Memory allocation failed.");
        }

        quiz->qas[quiz->size++] = qa;
        success = 1;
    }

    return success;
}

void shrink_quiz_size() {
    if (quiz->size) {
        quiz->capacity = quiz->size;
        quiz->qas = realloc(quiz->qas, quiz->capacity * sizeof(QAPair*));
        if(quiz->qas == NULL)
            print_message(FATAL, "Memory allocation failed.");
    }

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

    qa->question = NULL;
    qa->answer = NULL;
    
    if (!(parse_question(qa, line, tab) && parse_answer(qa, tab))) {
        free(qa->question);
        free(qa->answer);
        free(qa);
        qa = NULL;
    }
    
    return qa;
}

int parse_question(QAPair *qa, const char *line, char *tab) {
    char *trimmed;
    
    /* Creating the raw question string */
    qa->question = malloc(tab - line + 1);
    if (qa->question == NULL)
        print_message(FATAL, "Memory allocation failed.");
    
    /* since source is longer, no null terminator is copied, so we add it manually */
    strncpy(qa->question, line, tab - line);
    qa->question[tab - line] = '\0';

    /* Trimming the question, and deleting original */
    trimmed = trim(qa->question);
    free(qa->question);
    qa->question = trimmed;

    return *(qa->question);
}

int parse_answer(QAPair *qa, char *tab) {
    char *trimmed;

    /* Remove newline character from the end of the line (if present) */
    tab[strcspn(tab, "\n")] = '\0';

    qa->answer = malloc(strlen(tab + 1) + 1);
    if (qa->answer == NULL)
        print_message(FATAL, "Memory allocation failed.");
    
    /* This copies the null terminator as well */
    strcpy(qa->answer, tab + 1);
    
    /* Trimming the answer, and deleting original */
    trimmed = trim(qa->answer);
    free(qa->answer);
    qa->answer = trimmed;
    
    return *(qa->answer);
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
    int exit = 0;
    int range = quiz->size;
    if (range == 0)
        print_message(FATAL, "Could not read any data from the specified inputs.");
    
    welcome_toast(range);

    gamemode_select();

    print_message(QUESTION, "Press any key to start the quiz...");
    getchar_equals(0);

    /* clear any previous error messages */
    clear_screen();
    welcome_toast(range);
    print_message(INFO, "--------------------------------------------------------------------\n");

    start_timer();

    while (range && !exit) {
        QAPair *qa = random_question(range, &current);
        exit = ask_and_correct_question(qa);
        swap_qas(current, --range);

        /* If we are out of questions, we reset the range in INFINITE mode */
        if ((gamemode == INFINITE || gamemode == INFINITE_REVERSED) && range == 0)
            range = quiz->size;            
    }
    
    stop_timer();

    print_message(INFO, "--------------------------------------------------------------------\n");

    return;
}

