#pragma once
#include <dirent.h>
#include <stdio.h>

extern const char * const allowed_extensions[];
extern const int allowed_extensions_size;

void read_all_input(int argc, char *argv[]);

char *search_last_dot(char *input_string);

int extension_allowed(char *last_dot);

FILE* find_file(char *input_string);

DIR *find_folder(char *input_string);

void read_file(FILE *file);

void read_folder(DIR *folder);