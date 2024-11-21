#include <stdio.h>
#include "fs_read.h"
#include "quiz.h"
#include "evaluation.h"

int main(int argc, char *argv[]) {

    init_quiz();

    read_all_input(argc, argv);

    shrink_quiz_size();

    play_quiz();

    evaluate_quiz();

    free_quiz();

    return 0;
}
