#pragma once
#include <time.h>
#include "quiz.h"

extern time_t start, end, elapsed;

/**
 * Asks a question and corrects the user if the answer is wrong.
 * 
 * If the user types "!exit", the function will return 1, indicating that the user wants to exit.
 * 
 * @param qa Pointer to the question-answer pair
 * @return 1 if the user wants to exit, 0 otherwise
 */
int ask_and_correct_question(QAPair *qa);

/**
 * Starts the timer.
 */
void start_timer();

/**
 * Stops the timer and calculates the elapsed time.
 */
void stop_timer();

/**
 * Prints the time taken, the amount of correct answers, 
 * the amount of asked questions and the percentage of correct answers.
 */
void evaluate_quiz();