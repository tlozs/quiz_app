#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "fs_read.h"
#include "fs_utils.h"
#include "comm.h"
#include "quiz.h"

#define LINE_MAX 1024

const char * const input_root = "input";
const char * const allowed_extensions[] = {".txt", ".tsv"};
const int allowed_extensions_size = sizeof(allowed_extensions) / sizeof(allowed_extensions[0]);

void read_all_input(int argc, char *argv[]) {
    int i;

    if (no_input_root()) {
        print_message(ERROR, "Input directory not found.");
        print_message(QUESTION, "Would you like to create the input directory? (y/n) ");

        if (getchar_equals('y') && create_input_root()) {
            print_message(INFO, "Input directory created.");
            print_message(INFO, "Place your input files in the input directory, then press any key to continue.");
            print_message(QUESTION, "If there is no \033[1;35mANY\033[0m key on your keyboard, consult your local keyboard vendor...");
            getchar_equals(0);
        } 
        else {
            print_message(FATAL, "Could not create input directory.");
        }
        print_message(INFO, "");
    }

    if (argc == 1) {
        print_message(WARNING, "No input parameters specified in the command line.");
        print_message(QUESTION, "Would you like to read all files in the input directory? (y/n) ");
        if (getchar_equals('y'))
            try_read_input(path_join(input_root, NULL));
    }
    else
        for (i = 1; i < argc; i++)
            try_read_input(path_join(input_root, argv[i]));

    return;
}

void try_read_input(char *input_path) {
    if (!inside_input_root(input_path)) {
        print_message(ERROR, "Input '%s' is not within the input directory.", input_path);
    }
    else if (!try_read_file(input_path) && !try_read_folder(input_path)) {
        print_message(ERROR, "Could not read input path: '%s'", input_path);
    }

    free(input_path);
    return;
}

int try_read_file(const char *target_location) {
    char *dot = search_last_dot(target_location);
    FILE *file;
    int success = 0;

    if (dot && extension_allowed(dot) && (file = fopen(target_location, "r"))) {
        read_file(file, target_location);
        fclose(file);
        success = 1;
    }

    return success;
}

int try_read_folder(const char *target_location) {
    DIR *folder = opendir(target_location);
    int success = 0;

    if (folder) {
        read_folder(folder, target_location);
        closedir(folder);
        success = 1;
    }
    
    return success;
}

void read_file(FILE *file, const char *file_path) {
    char line[LINE_MAX];
    unsigned int line_number = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;

        if (sizeof(line) == strlen(line) + 1) {
            print_message(WARNING, "Unable to parse line %d of '%s': Line too long.", line_number, file_path);
            
            /* skip the rest until newline */
            while (fgetc(file) != '\n');
        }
        /* Check if the line is not just a newline character */
        else if (1 < strlen(line)) {
            if (format_correct(line)) {
                if (!extend_quiz(parse_to_qa(line)))
                    print_message(WARNING, "Unable to parse line %d of '%s': Question or answer is empty.", line_number, file_path);
            }
            else
                print_message(WARNING, "Unable to parse line %d of '%s': Incorrect format.", line_number, file_path);
        }
    }

    fclose(file);
    return;
}

void read_folder(DIR *folder, const char *folder_path) {
    struct dirent *entry;
    char *fname;

    while ((entry = readdir(folder))) {
        fname = entry->d_name;

        if (!(strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0)) {
            try_read_input(path_join(folder_path, fname));
        }
    }

    return;
}
