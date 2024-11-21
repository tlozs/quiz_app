#pragma once
#include "quiz.h"

/**
 * Asks a question and corrects the user if the answer is wrong.
 * 
 * @param qa Pointer to the question-answer pair
 */
void ask_and_correct_question(QAPair *qa);

void evaluate_quiz();