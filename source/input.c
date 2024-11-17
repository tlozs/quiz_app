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
    char *input_string;
    char *dot;
    FILE *file;
    DIR *folder;

    if (argc == 1) {
        printf("Error: no input files\n");
        /* return; */
    }

    for (i = 1; i < argc; i++) {
        input_string = argv[i];
        dot = search_last_dot(input_string);
        /* we try to read a file with the given name */
        if (dot && extension_allowed(dot) && (file = find_file(input_string))) read_file(file);
        /* if the file is not found, we try to read a folder with the same name */
        else if ((folder = find_folder(input_string))) read_folder(folder);
        else printf("Error: \'%s\' not found\n", input_string);
    }

    return;
}

char *search_last_dot(char *input_string) {
    return strrchr(input_string, '.');
}

int extension_allowed(char *last_dot) {
    int i;
    for (i = 0; i < allowed_extensions_size; i++)
        if (strcmp(last_dot, allowed_extensions[i]) == 0)
            return 1;
    return 0;
}

FILE *find_file(char *input_string) {
    FILE* file;
    char *file_location = malloc(strlen(input_location) + strlen(input_string) + 1);
    strcpy(file_location, input_location);
    strcat(file_location, input_string);
    file = fopen(file_location, "r");
    free(file_location);
    return file;
}

DIR *find_folder(char *input_string) {
    DIR *folder;
    char *folder_location = malloc(strlen(input_location) + strlen(input_string) + 1);
    strcpy(folder_location, input_location);
    strcat(folder_location, input_string);
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

void read_folder(DIR *folder) {
    struct dirent *entry;
    while ((entry = readdir(folder)))
        printf("%s\n", entry->d_name);
    closedir(folder);
    return;
}
