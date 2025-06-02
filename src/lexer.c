/**
 * @file lexer.c
 * @brief Lexical Analysis Implementation for Assembly Source
 *
 * Parses labels, directives, and operands from assembly source lines.
 * Handles syntax detection for instructions and data directives.
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
#include "lexer.h"
#include "utils.h"

/**
 * @brief Check if a line is a data or string directive.
 *
 * Validates whether the line includes a `.data` or `.string` directive,
 * optionally preceded by a label.
 * 
 * @param line Input line of assembly code
 * @return int 1 if line contains data/string directive, 0 otherwise
 */
int is_data_or_string_directive(const char *line) {
    return strstr(line, DATA_DIRECTIVE) || strstr(line, STRING_DIRECTIVE);
}

/**
 * @brief Extract opcode or directive token from a line.
 *
 * Parses the line to extract either an instruction opcode or
 * a directive starting with '.'.
 *
 * @param line Input line of assembly code
 * @param token_out Output buffer where token will be written (must be pre-allocated)
 * @return int 1 if a token was successfully extracted, 0 otherwise
 */
int extract_opcode_or_directive(const char *line, char *token_out) {
    const char *ptr = line;
    int i = 0;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;

    /* Copy characters into token buffer */
    while (*ptr && !isspace((unsigned char)*ptr) && i < MAX_LABEL_LENGTH) {
        token_out[i++] = *ptr++;
    }

    token_out[i] = '\0';
    return (i > 0);
}

/**
 * @brief Extract up to two operands from a line of code.
 *
 * Parses a line and extracts operands into a 2D array of strings.
 *
 * @param line Input assembly line
 * @param operands_out Output array for operands (2 strings of up to 31 chars + null)
 * @return int Number of operands found (0–2), or -1 on syntax error
 */
int extract_operands(const char *line, char operands_out[2][32]) {
    const char *ptr = line;
    int count = 0, i;

    /* Skip leading whitespace */
    while (*ptr && isspace((unsigned char)*ptr)) ptr++;

    for (i = 0; i < 2 && *ptr; i++) {
        int j = 0;

        /* Skip leading spaces before operand */
        while (*ptr && isspace((unsigned char)*ptr)) ptr++;

        /* Copy operand characters */
        while (*ptr && *ptr != ',' && !isspace((unsigned char)*ptr) && j < 31) {
            operands_out[i][j++] = *ptr++;
        }

        operands_out[i][j] = '\0';

        if (j > 0) {
            count++;
        }

        /* Skip trailing spaces and comma */
        while (*ptr && (*ptr == ' ' || *ptr == ',' || *ptr == '\t')) ptr++;
    }

    return count;
} 
