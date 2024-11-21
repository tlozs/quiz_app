#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fs_read.h"
#include "quiz.h"
#include "evaluation.h"
#include "comm.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    clear_screen();
    /* TODO: make things const */
    /* TODO: all functions and globals in header? */
    init_quiz();

    read_all_input(argc, argv);

    shrink_quiz_size();

    play_quiz();

    evaluate_quiz();

    free_quiz();

    return 0;
}
