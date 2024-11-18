#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include "input.h"

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

int no_input_root() {
    DIR *dir = opendir(input_root);
    if (dir) {
        closedir(dir);
        return 0;
    }
    else return 1;
}

void create_input_root() {
    mkdir(input_root);
    return;
}

char *search_last_dot(const char *path_w_filename) {
    return strrchr(path_w_filename, '.');
}

int extension_allowed(const char *last_dot) {
    int i;
    for (i = 0; i < allowed_extensions_size; i++)
        if (strcmp(last_dot, allowed_extensions[i]) == 0)
            return 1;
    return 0;
}

char *path_join(const char* const path, const char* const target) {
    char *target_path;
    if (target == NULL)
        target_path = strdup(path);
    else {
        target_path = malloc(strlen(path) + strlen(target) + 3);
        sprintf(target_path, "%s\\%s", path, target);
    }
    return target_path;
}

int inside_input_root(const char *path) {
    /* The absolute resolved path to the target (without references and symlinks) */
    char resolved_path[PATH_MAX];
    /* The path to the current working directory */
    char cwd[PATH_MAX];
    /* The full path to the input location (cwd + input_root) */
    char *full_input_location;
    /* whether the path starts with the input location */
    int result;

    /* If the path is not resolved or the current working directory is not found, return 0 */
    if (!(_fullpath(resolved_path, path, PATH_MAX) && getcwd(cwd, sizeof(cwd))))
        return 0;

    full_input_location = path_join(cwd, input_root);

    result = strncmp(resolved_path, full_input_location, strlen(full_input_location)) == 0;

    free(full_input_location);
    return result;
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
    if (dot && extension_allowed(dot) && (file = find_file(target_location))) {
        read_file(file);
        return 1;
    }
    return 0;
}

int try_read_folder(const char *target_location) {
    DIR *folder = find_folder(target_location);
    if (folder) {
        read_folder(folder, target_location);
        return 1;
    }
    return 0;
}

FILE *find_file(const char *file_location) {
    return fopen(file_location, "r");
}

DIR *find_folder(const char *folder_location) {
    return opendir(folder_location);
}

void read_file(FILE *file) {
    char c;
    while ((c = fgetc(file)) != EOF)
        putchar(c);
    fclose(file);
    printf("\n");
    return;
}

void read_folder(DIR *folder, const char *folder_path) {
    struct dirent *entry;
    char *folder_path_extended;
    char *fname;

    /* we read all the files/foldersin the folder */
    while ((entry = readdir(folder))) {
        fname = entry->d_name;

        /* we need to skip the current and parent directories */
        if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0) continue;
        
        /* 
        since we are in the folder, we need to add the folder name to the searched file/folder
        (instead of 'input/fname.txt' we need to read 'input/<we_are_in_this_folder>/fname.txt')
        */
        folder_path_extended = path_join(folder_path, fname);

        /* try to read entry as a file, if it fails it should be a folder so we recursively search it further */
        try_read_input(folder_path_extended);
    }
    closedir(folder);
    return;
}
