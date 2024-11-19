#pragma once
#include <dirent.h>
#include <stdio.h>

/* Location of the input files and folders relative to the executable */
extern const char * const input_root;

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
 * Tries to read the input first as a file, or if no file with the given name is found, as a folder.
 * 
 * If neither a file nor a folder is found, prints an error message.
 * 
 * The input is searched relative to the defined input_location constant, 
 * and the input is considered valid if it is within the input_location directory.
 * 
 * @param input Path to the input file or folder, including the input_location
 */
void try_read_input(char *input_path);

/**
 * Tries to find and read a file at the given location, when all criteria are met.
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

/* TODO: update later */
/**
 * Reads the contents of a file and prints it to the standard output.
 * 
 * @param file Pointer to the file to be read
 */
void read_file(FILE *file);

/**
 * Reads all the contents of a folder and recursively reads all files and folders within it.
 * 
 * Skips reading the current and parent directory entries.
 * 
 * To the recursive calls, extends the folder path with the name of the current entry.
 * (instead of 'input/fname.txt' we need to read 'input/<we_are_in_this_folder>/fname.txt')
 * 
 * @param folder Pointer to the folder to be read
 * @param folder_path Path to the folder
 */
void read_folder(DIR *folder, const char *folder_path);