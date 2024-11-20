#pragma once

typedef struct {
    char *question;
    char *answer;
} QAPair;

typedef struct {
    QAPair **qas;
    int size;
    int capacity;
} Quiz;

extern Quiz *quiz;

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
 * Frees all memory allocated for the quiz.
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
 * Plays the quiz by printing each question and answer in the quiz.
 */
void play_quiz();
