/**
 * @file lexer.h
 * @brief Lexical Analysis Interface for Assembly Tokens
 *
 * Provides core functions for extracting and validating assembly components,
 * such as labels, directives, operands, and arguments.
 * Used in both the first and second passes.
 * 
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "globals.h"

/**
 * @brief Check if a line is a data or string directive.
 *
 * Validates whether the line includes a `.data` or `.string` directive,
 * optionally preceded by a label.
 * 
 * @param line Input line of assembly code
 * @return int 1 if line contains data/string directive, 0 otherwise
 */
int is_data_or_string_directive(const char *line);

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
int extract_opcode_or_directive(const char *line, char *token_out);

/**
 * @brief Extract up to two operands from a line of code.
 *
 * Parses a line and extracts operands into a 2D array of strings.
 *
 * @param line Input assembly line
 * @param operands_out Output array for operands (2 strings of up to 31 chars + null)
 * @return int Number of operands found (0–2), or -1 on syntax error
 */
int extract_operands(const char *line, char operands_out[2][32]);

#endif /* LEXER_H */ 
