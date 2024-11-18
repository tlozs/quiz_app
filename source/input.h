#pragma once
#include <dirent.h>
#include <stdio.h>

/* Location of the input files and folders */
extern const char * const input_root;

/* Allowed file extensions */
extern const char * const allowed_extensions[];

/* Number of allowed file extensions */
extern const int allowed_extensions_size;

/**
 * Checks if the input directory exists.
 * 
 * @return 1 if the input directory is not found, 0 otherwise
 */
int no_input_root();

/**
 * Creates the input directory.
 */
void create_input_root();

/**
 * Reads all input files and folders specified in the command line arguments.
 * 
 * If no arguments are provided, prompts the user to read all files in the input folder.
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 */
void read_all_input(int argc, char *argv[]);

/**
 * Searches for the last dot in the given file path.
 * 
 * @param path_w_filename Path to the file
 * @return Pointer to the last dot in the file path, or NULL if no dot is found
 */
char *search_last_dot(const char *path_w_filename);

/**
 * Checks if the file extension is allowed.
 * 
 * Essentially, checks if the extension (string after the last dot) is in the allowed extensions list.
 * 
 * @param last_dot Pointer to the last dot in the file path
 * @return 1 if the extension is allowed, 0 otherwise
 */
int extension_allowed(const char *last_dot);

/**
 * Joins two paths together.
 * 
 * If target is NULL, returns a copy of the first path.
 * 
 * @param path First path
 * @param target Second path
 * @return Pointer to the joined path, should be freed after use
 */
char *path_join(const char* const path, const char* const target);

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

/**
 * Tries to read the input first as a file, or if no file with the given name is found, as a folder.
 * 
 * If neither a file nor a folder is found, prints an error message.
 * 
 * The input is treated relative to the defined input_location constant, 
 * and the input is considered valid if it is within the input directory.
 * 
 * @param input Path to the input file or folder
 */
void try_read_input(char *input_path);

/**
 * Tries to find and read a file at the given location.
 * 
 * Applies the following checks:
 * 
 * - The file has an extension
 * 
 * - The extension is allowed
 * 
 * - The file is found
 * 
 * @param target_location Path to the file
 * @return 1 if the file is successfully read, 0 otherwise
 */
int try_read_file(const char *target_location);

/**
 * Tries to find and read a folder at the given location.
 * 
 * @param target_location Path to the folder
 * @return 1 if the folder is successfully read, 0 otherwise
 */
int try_read_folder(const char *target_location);

/**
 * Finds and opens a file at the given location.
 * 
 * @param file_location Path to the file
 * @return Pointer to the opened file, or NULL if the file is not found
 */
FILE* find_file(const char *file_location);

/**
 * Finds and opens a folder at the given location.
 * 
 * @param folder_location Path to the folder
 * @return Pointer to the opened folder, or NULL if the folder is not found
 */
DIR *find_folder(const char *folder_location);

/**
 * Reads the contents of a file and prints it to the standard output.
 * 
 * @param file Pointer to the file to be read
 */
void read_file(FILE *file);

/**
 * Reads the contents of a folder and recursively reads all files and folders within it.
 * 
 * @param folder Pointer to the folder to be read
 * @param folder_path Path to the folder
 */
void read_folder(DIR *folder, const char *folder_path);