/**
 * @file text_parser.h
 * @brief Interface for Assembly Text Parsing and Validation
 *
 * Provides functions for extracting labels, directives, operands,
 * parsing arguments for .data/.string, validating identifiers and numbers.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#ifndef TEXT_PARSER_H
#define TEXT_PARSER_H

#include "globals.h"

/* -------------------------
   Extraction Functions
   ------------------------- */

/**
 * @brief Skip whitespace characters starting at given index.
 * @param str Input string
 * @param pos Pointer to index (modified in-place)
 */
void skip_whitespace(const char *str, int *pos);

/**
 * @brief Extract a label if it appears at current position.
 * @param str Input line
 * @param pos Pointer to index (modified)
 * @return Dynamically allocated label, or NULL
 */
char *extract_label(const char *str, int *pos);

/**
 * @brief Extract a directive (starts with '.') from current position.
 * @param str Input line
 * @param pos Pointer to index (modified)
 * @return Allocated directive string or NULL
 */
char *extract_directive(const char *str, int *pos);

/**
 * @brief Extract all remaining arguments (e.g. after directive).
 * @param str Input line
 * @param pos Pointer to index (modified)
 * @return Allocated string with arguments
 */
char *extract_arguments(const char *str, int *pos);

/**
 * @brief Copy a substring from str[pos] with n characters.
 * @param str Source string
 * @param pos Start index
 * @param n Number of characters
 * @return Allocated substring
 */
char *extract_chars(const char *str, int pos, int n);

/* -------------------------
   .data / .string Parsing
   ------------------------- */

/**
 * @brief Parse a .data line into integer values
 * @param str Argument string
 * @param values Output array
 * @param max_values Max values allowed
 * @return Count of parsed values, or -1 on error
 */
int parse_data_values(const char *str, int *values, int max_values);

/**
 * @brief Parse a .string argument into ASCII values
 * @param str Argument string
 * @param values Output array
 * @param max_values Maximum output values
 * @return Number of characters parsed (including \0), or -1
 */
int parse_string_value(const char *str, int *values, int max_values);

/**
 * @brief Count how many values are in a .data argument
 * @param str Argument string
 * @return Number of values or -1 on syntax error
 */
int count_data_values(const char *str);

/* -------------------------
   Validation Functions
   ------------------------- */

/**
 * @brief Check if given label is valid
 * @param str Label name
 * @return 1 if valid, 0 otherwise
 */
int is_valid_label(const char *str);

/**
 * @brief Check if a string is a valid number (int)
 * @param str Input string
 * @return 1 if valid, 0 otherwise
 */
int is_number(const char *str);

/**
 * @brief Check if character is space or tab
 * @param c Character
 * @return 1 if whitespace, 0 otherwise
 */
int is_space_or_tab(char c);

/* -------------------------
   Utility
   ------------------------- */

/**
 * @brief Remove a comment (starting with ';') from a line
 * @param str Input line (modified in-place)
 */
void remove_comment(char *str);

#endif /* TEXT_PARSER_H */ 
