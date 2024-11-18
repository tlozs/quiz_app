#pragma once
#include <dirent.h>
#include <stdio.h>

/* Location of the input files and folders */
extern const char * const input_location;

/* Allowed file extensions */
extern const char * const allowed_extensions[];

/* Number of allowed file extensions */
extern const int allowed_extensions_size;

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
char *search_last_dot(char *path_w_filename);

/**
 * Checks if the file extension is allowed.
 * 
 * Essentially, checks if the extension (string after the last dot) is in the allowed extensions list.
 * 
 * @param last_dot Pointer to the last dot in the file path
 * @return 1 if the extension is allowed, 0 otherwise
 */
int extension_allowed(char *last_dot);

/**
 * Tries to read the input first as a file, or if no file with the given name is found, as a folder.
 * 
 * If neither a file nor a folder is found, prints an error message.
 * 
 * @param input Path to the input file or folder
 */
void try_read_input(char *input);

/**
 * Tries to read the input as a file.
 * 
 * Applies the following checks:
 * 
 * - The file has an extension
 * 
 * - The extension is allowed
 * 
 * - The file is found
 * 
 * @param path_w_filename Path to the file
 * @return 1 if the file is successfully read, 0 otherwise
 */
int try_read_file(char *path_w_filename);

/**
 * Tries to read the input as a folder.
 * 
 * @param path_w_foldername Path to the folder
 * @return 1 if the folder is successfully read, 0 otherwise
 */
int try_read_folder(char *path_w_foldername);

/**
 * Finds and opens a file at the given location.
 * 
 * Location is relative to the specified input_location constant.
 * 
 * @param path_w_filename Path to the file
 * @return Pointer to the opened file, or NULL if the file is not found
 */
FILE* find_file(char *path_w_filename);

/**
 * Finds and opens a folder in the input location.
 * 
 * Location is relative to the specified input_location constant.
 * 
 * @param path_w_foldername Path to the folder
 * @return Pointer to the opened folder, or NULL if the folder is not found
 */
DIR *find_folder(char *path_w_foldername);

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
void read_folder(DIR *folder, char *folder_path);