#pragma once
#include <dirent.h>
#include <stdio.h>

extern const char * const allowed_extensions[];
extern const int allowed_extensions_size;

void read_all_input(int argc, char *argv[]);

char *search_last_dot(char *path_w_filename);

int extension_allowed(char *last_dot);

void try_read_input(char *input);

int try_read_file(char *path_w_filename);

int try_read_folder(char *path_w_foldername);

FILE* find_file(char *path_w_filename);

DIR *find_folder(char *path_w_foldername);

void read_file(FILE *file);

void read_folder(DIR *folder, char *folder_path);