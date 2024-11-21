#pragma once

typedef enum message_type {
    INFO,
    QUESTION,
    INCORRECT,
    WARNING,
    ERROR,
    FATAL
} message_type;

/**
 * Prints a welcome message to the console.
 */
void welcome_toast();

/**
 * Prints a prefixed and colored message to the console.
 * 
 * If the message type is FATAL, the program will exit after printing the message.
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
 * If the parameter is 0, it works as a Press Any Key function.
 * 
 * @param c The character to compare to
 * @return char The character received
 */
int getchar_equals(char c);

/**
 * Clears the console screen.
 */
void clear_screen();