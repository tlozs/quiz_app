#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "fs_read.h"
#include "fs_utils.h"

const char * const input_root = "input";
const char * const allowed_extensions[] = {".txt", ".csv", ".tsv"};
const int allowed_extensions_size = sizeof(allowed_extensions) / sizeof(allowed_extensions[0]);

void read_all_input(int argc, char *argv[]) {
    int i;
    char c;

    if (no_input_root())
    {
        printf("\033[1;31mError: input directory not found\033[0m\n");
        printf("Would you like to create the input directory? (y/n) ");
        c = getchar();
        while (getchar() != '\n');
        
        if (c == 'y' || c == 'Y') {
            create_input_root();
            printf("Input directory created\n");
            printf("\nPlace your input files in the input directory, then press any key to continue\n");
            printf("(If there is no \033[1;35mANY\033[0m key on your keyboard, consult your local keyboard vendor) ");
            while ((c = getchar()) != '\n');
        }
    }

    if (argc == 1) {
        printf("\033[1;33mWarning: no input parameters specified in the command line\033[0m\n");
        printf("Would you like to read all files in the input directory? (y/n) ");
        c = getchar();
        while (getchar() != '\n');
        if (c == 'y' || c == 'Y')
            try_read_input(path_join(input_root, NULL));
        else
            printf("\033[1;31mError: No input files read\033[0m\n");
    }
    else
        for (i = 1; i < argc; i++)
            try_read_input(path_join(input_root, argv[i]));

    return;
}

void try_read_input(char *input_path) {
    if (inside_input_root(input_path)) {
        if (!try_read_file(input_path) && !try_read_folder(input_path))
            printf("\033[1;31mError: could not read \'%s\'\033[0m\n", input_path);
    }
    else
        printf("\033[1;31mError: input \'%s\' is not within the input directory\033[0m\n", input_path);

    free(input_path);
    return;
}

int try_read_file(const char *target_location) {
    char *dot = search_last_dot(target_location);
    FILE *file;
    if (dot && extension_allowed(dot) && (file = fopen(target_location, "r"))) {
        read_file(file);
        fclose(file);
        return 1;
    }
    return 0;
}

int try_read_folder(const char *target_location) {
    DIR *folder = opendir(target_location);
    if (folder) {
        read_folder(folder, target_location);
        closedir(folder);
        return 1;
    }
    return 0;
}

void read_file(FILE *file) {
    char c;
    while ((c = fgetc(file)) != EOF)
        putchar(c);
    printf("\n");
    return;
}

void read_folder(DIR *folder, const char *folder_path) {
    struct dirent *entry;
    char *folder_path_extended;
    char *fname;

    while ((entry = readdir(folder))) {
        fname = entry->d_name;
        if (!(strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0)) {
            folder_path_extended = path_join(folder_path, fname);
            try_read_input(folder_path_extended);
        }
    }
    return;
}
