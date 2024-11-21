#pragma once

typedef struct QAPair {
    char *question;
    char *answer;
} QAPair;

typedef struct Quiz {
    QAPair **qas;
    int size;
    int capacity;
} Quiz;

typedef enum GameMode {
    INFINITE,
    INFINITE_REVERSED,
    ONEROUNDER,
    ONEROUNDER_REVERSED
} GameMode;

/* Global variable for the quiz containing the question-answer pairs and metadata */
extern Quiz *quiz;

/* Global variable for tracking the game mode */
extern GameMode gamemode;

/**
 * Initializes the quiz with an empty question-answer pair array and an initial capacity.
 */
void init_quiz();

/**
 * Extends the quiz with a new question-answer pair.
 * 
 * If the quiz is full, the capacity is doubled, and the quiz is reallocated.
 * 
 * @param qa Question-answer pair to add to the quiz
 */
void extend_quiz(QAPair *qa);

/**
 * Shrinks the quiz capacity (and memory usage) to the current size.
 * 
 * If the quiz is empty, the function will not crash.
 */
void shrink_quiz_size();

/**
 * Frees all memory allocated for the quiz.
 * 
 * No need to check for NULL pointers, as the function will not crash if the quiz is empty.
 */
void free_quiz();

/**
 * Parses a line into a question-answer pair.
 * 
 * The line is expected to have the following format:
 * 
 * - The question is separated from the answer by a tab character
 * 
 * @param line Line to parse
 * @return Pointer to the parsed question-answer pair
 */
QAPair* parse_to_qa(const char *line);

/**
 * Parses the question part of an input line into a question-answer pair.
 * 
 * @param qa Pointer to the question-answer pair
 * @param line Line to parse
 * @param tab Pointer to the tab character in the line
 */
void parse_question(QAPair *qa, const char *line, char *tab);

/**
 * Parses the answer part of an input line into a question-answer pair.
 * 
 * @param qa Pointer to the question-answer pair
 * @param tab Pointer to the tab character in the line
 */
void parse_answer(QAPair *qa, char *tab);

/**
 * Gets a random question from the quiz.
 * 
 * @param range The range of the random number
 * @param out_index Pointer to the index of the question in the quiz
 * @return Pointer to the random question
 */
QAPair *random_question(int range, int *out_index);

/**
 * Swaps two question-answer pairs in the quiz.
 * 
 * @param i Index of the first question-answer pair
 * @param j Index of the second question-answer pair
 */
void swap_qas(int i, int j);

/**
 * Main gameplay loop. Asks random questions from the quiz.
 * 
 * After each question, the used question is swapped to the end of the quiz
 * and the pool of questions is reduced by one. This method ensures that no
 * question is asked twice in the same quiz session.
 */
void play_quiz();
