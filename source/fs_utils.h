#pragma once

/**
 * Checks if the given line has the correct format.
 * 
 * The correct format is defined as:
 * 
 * - The line has exactly one tab character
 * 
 * @param line Line to check
 * @return 1 if the line has the correct format, 0 otherwise
 */
int format_correct(const char *line);

/**
 * Checks if the input directory exists by trying to read it.
 * 
 * @return 1 if the input directory is not found, 0 otherwise
 */
int no_input_root();

/**
 * Creates a directory at the input location.
 * 
 * @return 1 if the directory is successfully created, 0 otherwise
 */
int create_input_root();

/**
 * Searches for the last dot in the given string.
 * 
 * @param string Where to search for the last dot
 * @return Pointer to the last dot in the string, or NULL if no dot is found
 */
char *search_last_dot(const char *string);

/**
 * Checks if the given line has at least one tab character, 
 * and the first and last tab characters in the line are the same.
 * 
 * @param line Line to check
 * @return 1 if the line has only one tab character, 0 otherwise
 */
int exactly_one_tab(const char *line);

/**
 * Checks if the file extension is one of the allowed extensions.
 * 
 * @param extension Pointer to the last dot in the file path (essentially the extension as a string)
 * @return 1 if the extension is allowed, 0 otherwise
 */
int extension_allowed(const char *extension);

/**
 * Joins two paths with a \ sign together.
 * 
 * If target is NULL, returns a copy of the first path.
 * 
 * @param path First path
 * @param target Second path
 * @return Pointer to the joined path, should be freed after use
 */
char *path_join(const char* path, const char* target);

/**
 * Checks if the given path is within the input directory.
 * 
 * Resolving takes care of ../ references and symlinks.
 * 
 * The path is considered within the input directory if it starts with the input directory path.
 * 
 * @param path Path to the file or folder
 * @return 1 if the path is within the input directory, 0 otherwise
 */
int inside_input_root(const char *path);