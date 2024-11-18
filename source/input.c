#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "input.h"

const char * const input_location = "input/";
const char * const allowed_extensions[] = {".txt", ".csv", ".tsv"};
const int allowed_extensions_size = sizeof(allowed_extensions) / sizeof(allowed_extensions[0]);

void read_all_input(int argc, char *argv[]) {
    int i;
    char c;

    if (argc == 1) {
        printf("Warning: no input files specified in the command line\n");
        printf("Would you like to read all files in the input folder? (y/n) ");
        c = getchar();
        if (c == 'y' || c == 'Y') {
            try_read_input("");
        }
        else
            printf("No input files read\n");
    }
    else
        for (i = 1; i < argc; i++)
            try_read_input(argv[i]);

    return;
}

char *search_last_dot(char *path_w_filename) {
    return strrchr(path_w_filename, '.');
}

int extension_allowed(char *last_dot) {
    int i;
    for (i = 0; i < allowed_extensions_size; i++)
        if (strcmp(last_dot, allowed_extensions[i]) == 0)
            return 1;
    return 0;
}

void try_read_input(char *input) {
    if (!try_read_file(input))
        if (!try_read_folder(input))
            printf("Error: could not read \'%s\'\n", input);
    return;
}

int try_read_file(char *path_w_filename) {
    char *dot = search_last_dot(path_w_filename);
    FILE *file;
    if (dot && extension_allowed(dot) && (file = find_file(path_w_filename))) {
        read_file(file);
        return 1;
    }
    return 0;
}

int try_read_folder(char *path_w_foldername) {
    DIR *folder;
    if ((folder = find_folder(path_w_foldername))) {
        read_folder(folder, path_w_foldername);
        return 1;
    }
    return 0;
}

FILE *find_file(char *path_w_filename) {
    FILE* file;
    char *file_location = malloc(strlen(input_location) + strlen(path_w_filename) + 1);
    strcpy(file_location, input_location);
    strcat(file_location, path_w_filename);
    file = fopen(file_location, "r");
    free(file_location);
    return file;
}

DIR *find_folder(char *path_w_foldername) {
    DIR *folder;
    char *folder_location = malloc(strlen(input_location) + strlen(path_w_foldername) + 1);
    strcpy(folder_location, input_location);
    strcat(folder_location, path_w_foldername);
    folder = opendir(folder_location);
    free(folder_location);
    return folder;
}

void read_file(FILE *file) {
    char c;
    while ((c = fgetc(file)) != EOF)
        putchar(c);
    fclose(file);
    printf("\n");
    return;
}

void read_folder(DIR *folder, char *folder_path) {
    struct dirent *entry;
    char *folder_path_extended;
    char *fname;

    /* we read all the files/foldersin the folder */
    while ((entry = readdir(folder))) {
        fname = entry->d_name;

        /* we need to skip the current and parent directories */
        if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0) continue;
        
        /* 
        we search relative to the input location, 
        so we need to add the folder name to the searched file, since we are in the folder
        (instead of 'input/a.txt' we need to read 'input/<we_are_in_this_folder>/a.txt')
        */
        folder_path_extended = malloc(strlen(folder_path) + strlen(fname) + 2);
        strcpy(folder_path_extended, folder_path);
        strcat(folder_path_extended, "/");
        strcat(folder_path_extended, fname);

        /* try to read entry as a file, if it fails it should be a folder so we recursively search it further */
        try_read_input(folder_path_extended);
        
        free(folder_path_extended);
    }
    closedir(folder);
    return;
}
