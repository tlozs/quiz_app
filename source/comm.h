#pragma once

typedef enum {
    INFO,
    QUESTION,
    WARNING,
    ERROR
} message_type;

/**
 * Prints a message to the console.
 * 
 * @param type The type of message to print
 * @param message The message to print
 */
void print_message(message_type type, const char *message, ...);

/**
 * Gets a character from the console and case insensitively checks
 * if it is equal to the given character.
 * 
 * After checking, it will discard any remaining characters 
 * in the input buffer, until a newline is found.
 * 
 * @param c The character to compare to
 * @return char The character received
 */
int getchar_equals(char c);