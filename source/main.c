#include <stdio.h>
#include "fs_read.h"
#include "quiz.h"
#include "evaluation.h"

int main(int argc, char *argv[]) {
    
    read_all_input(argc, argv);

    play_quiz();

    evaluate_quiz();

    return 0;
}
