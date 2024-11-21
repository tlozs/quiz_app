#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include "fs_read.h"
#include "fs_utils.h"
#include "comm.h"

int format_correct(const char *line) {
    return exactly_one_tab(line);
}

int no_input_root() {
    DIR* input_dir = opendir(input_root);
    int result = input_dir == NULL;

    if (input_dir)
        closedir(input_dir);

    return result;
}

int create_input_root() {
    return mkdir(input_root) == 0;
}

char *search_last_dot(const char *string) {
    return strrchr(string, '.');
}

int exactly_one_tab(const char *line) {
    char *first_tab = strchr(line, '\t');
    char *last_tab = strrchr(line, '\t');

    return first_tab != NULL && first_tab == last_tab;
}

int extension_allowed(const char *extension) {
    int i;

    for (i = 0; i < allowed_extensions_size; i++)
        if (strcmp(extension, allowed_extensions[i]) == 0)
            return 1;

    return 0;
}

char *path_join(const char* const path, const char* const target) {
    char *target_path;

    if (target == NULL)
        target_path = strdup(path);
    else {
        target_path = malloc(strlen(path) + strlen(target) + 2);
        if (target_path == NULL)
            print_message(FATAL, "Memory allocation failed.");

        sprintf(target_path, "%s\\%s", path, target);
    }

    return target_path;
}

int inside_input_root(const char *path) {
    /* The absolute resolved path to the target (without references and symlinks) */
    char resolved_path[PATH_MAX];
    /* The path to the current working directory */
    char cwd[PATH_MAX];
    /* The absolute path to the input location (cwd + input_root) */
    char *full_input_location;
    /* whether the path starts with the input location */
    int result = 0;

    /* If the path is resolved and the current working directory is found */
    if (_fullpath(resolved_path, path, PATH_MAX) && getcwd(cwd, sizeof(cwd))) {
        full_input_location = path_join(cwd, input_root);
        result = strncmp(resolved_path, full_input_location, strlen(full_input_location)) == 0;
        free(full_input_location);
    }

    return result;
}