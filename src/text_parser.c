/**
 * @file text_parser.c
 * @brief Implementation for text parsing and validation in the assembler
 *
 * Includes functions to:
 * - Extract labels and directives
 * - Parse data and string arguments
 * - Validate label syntax and numeric formats
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "globals.h"
#include "text_parser.h"
#include "utils.h"
#include "errors.h"

/* -------------------------
   Extraction Functions
   ------------------------- */

/**
 * @brief Skip whitespace characters starting at given index.
 * @param str Input string
 * @param pos Pointer to index (modified in-place)
 */
void skip_whitespace(const char *str, int *pos) {
    while (is_space_or_tab(str[*pos])) {
        (*pos)++;
    }
}

/**
 * @brief Extract a label if it appears at current position.
 * @param str Input line
 * @param pos Pointer to index (modified)
 * @return Dynamically allocated label, or NULL
 */
char *extract_label(const char *str, int *pos) {
    int start, len;
    char *label;

    skip_whitespace(str, pos);

    /* Ensure first character is a letter */
    if (!isalpha((unsigned char)str[*pos])) return NULL;

    start = *pos;
    while (isalnum((unsigned char)str[*pos]) || str[*pos] == '_') {
        (*pos)++;
    }

    /* Require colon after label */
    if (str[*pos] != ':') {
        *pos = start;
        return NULL;
    }

    len = *pos - start;
    label = (char *)malloc(len + 1);
    strncpy(label, str + start, len);
    label[len] = '\0';
    (*pos)++; /* Skip colon */

    return label;
}

/**
 * @brief Extract a directive (starts with '.') from current position.
 * @param str Input line
 * @param pos Pointer to index (modified)
 * @return Allocated directive string or NULL
 */
char *extract_directive(const char *str, int *pos) {
    int start, len;
    char *directive;

    skip_whitespace(str, pos);
    if (str[*pos] != '.') return NULL;

    start = *pos;
    while (str[*pos] && !is_space_or_tab(str[*pos]) && str[*pos] != '\n') {
        (*pos)++;
    }

    len = *pos - start;
    directive = (char *)malloc(len + 1);
    strncpy(directive, str + start, len);
    directive[len] = '\0';

    return directive;
}

/**
 * @brief Extract all remaining arguments (e.g. after directive).
 * @param str Input line
 * @param pos Pointer to index (modified)
 * @return Allocated string with arguments
 */
char *extract_arguments(const char *str, int *pos) {
    int start, len;
    char *args;

    skip_whitespace(str, pos);
    if (!str[*pos] || str[*pos] == '\n') return NULL;

    start = *pos;
    while (str[*pos] && str[*pos] != '\n') {
        (*pos)++;
    }

    len = *pos - start;
    args = (char *)malloc(len + 1);
    strncpy(args, str + start, len);
    args[len] = '\0';

    return args;
}
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
int parse_data_values(const char *str, int *values, int max_values) {
    int pos = 0, count = 0;
    long value;
    char *endptr;

    skip_whitespace(str, &pos);

    while (str[pos] && count < max_values) {
        value = strtol(str + pos, &endptr, 10);

        /* Validate numeric parsing */
        if (endptr == str + pos || value < MIN_CONTENT || value > MAX_CONTENT) return -1;

        values[count++] = (int)value;
        pos = endptr - str;

        skip_whitespace(str, &pos);
        if (str[pos] == ',') {
            pos++;
            skip_whitespace(str, &pos);
        } else if (str[pos]) {
            return -1;
        }
    }

    return count;
}

/**
 * @brief Parse a .string argument into ASCII values
 * @param str Argument string
 * @param values Output array
 * @param max_values Maximum output values
 * @return Number of characters parsed (including \0), or -1
 */
int parse_string_value(const char *str, int *values, int max_values) {
    int pos = 0, count = 0;

    skip_whitespace(str, &pos);
    if (str[pos++] != '"') return -1;

    while (str[pos] && str[pos] != '"' && count < max_values - 1) {
        values[count++] = (unsigned char)str[pos++];
    }

    if (str[pos] != '"') return -1;

    values[count++] = '\0'; /* Append null terminator */
    return count;
}

/**
 * @brief Count how many values are in a .data argument
 * @param str Argument string
 * @return Number of values or -1 on syntax error
 */
int count_data_values(const char *str) {
    int pos = 0, count = 0;

    skip_whitespace(str, &pos);
    while (str[pos]) {
        /* Skip leading whitespace or commas */
        if (str[pos] == ',') {
            pos++;
            skip_whitespace(str, &pos);
            continue;
        }

        if (!isdigit((unsigned char)str[pos]) &&
            str[pos] != '+' && str[pos] != '-') return -1;

        /* Parse and skip current number */
        while (isdigit((unsigned char)str[pos]) || str[pos] == '+' || str[pos] == '-') pos++;

        count++;
        skip_whitespace(str, &pos);

        if (str[pos] == ',') {
            pos++;
            skip_whitespace(str, &pos);
        } else if (str[pos] && str[pos] != '\n') {
            return -1;
        }
    }

    return count;
}

/* -------------------------
   Validation Functions
   ------------------------- */

/**
 * @brief Check if given label is valid
 * @param str Label name
 * @return 1 if valid, 0 otherwise
 */
int is_valid_label(const char *str) {
    int i, len;
    if (!str || !isalpha((unsigned char)str[0])) return 0;

    len = strlen(str);
    if (len > MAX_LABEL_LENGTH) return 0;

    for (i = 1; i < len; i++) {
        if (!isalnum((unsigned char)str[i]) && str[i] != '_') return 0;
    }

    return 1;
}

/**
 * @brief Check if a string is a valid number (int)
 * @param str Input string
 * @return 1 if valid, 0 otherwise
 */
int is_number(const char *str) {
    if (!str || !*str) return 0;
    if (*str == '+' || *str == '-') str++;

    while (*str) {
        if (!isdigit((unsigned char)*str)) return 0;
        str++;
    }

    return 1;
}

/**
 * @brief Check if character is space or tab
 * @param c Character
 * @return 1 if whitespace, 0 otherwise
 */
int is_space_or_tab(char c) {
    return (c == ' ' || c == '\t');
}

/* -------------------------
   Utility
   ------------------------- */

/**
 * @brief Remove a comment (starting with ';') from a line
 * @param str Input line (modified in-place)
 */
void remove_comment(char *str) {
    char *semicolon = strchr(str, COMMENT_CHAR);
    if (semicolon) *semicolon = '\0';
} 
