#pragma once
#include <time.h>
#include "quiz.h"

extern time_t start, end, elapsed;

/**
 * Asks a question and corrects the user if the answer is wrong.
 * 
 * @param qa Pointer to the question-answer pair
 */
void ask_and_correct_question(QAPair *qa);

/**
 * Starts the timer.
 */
void start_timer();

/**
 * Stops the timer and calculates the elapsed time.
 */
void stop_timer();

/**
 * Evaluates the quiz.
 */
void evaluate_quiz();