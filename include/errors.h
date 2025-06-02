/**
 * @file errors.h
 * @brief Error Reporting Interface
 *
 * Declares the error handling API used across all assembler phases.
 * Enables contextual error reporting with file name and line number.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

/*-----------------------------------------------------------------------------
  Error Type Enumeration
  ---------------------------------------------------------------------------*/

/**
 * @enum ErrorType
 * @brief Classifies error sources for consistent reporting
 */
typedef enum {
    ERROR_NONE,            /**< No error occurred */
    ERROR_FILE,            /**< File I/O error */
    ERROR_MEMORY,          /**< Memory allocation failure */
    ERROR_SYNTAX,          /**< Syntax error in input */
    ERROR_RANGE,           /**< Value out of bounds */
    ERROR_SYMBOL,          /**< Symbol table or reference error */
    ERROR_DIRECTIVE,       /**< Invalid or unknown directive */
    ERROR_MACRO,           /**< Macro-related error */
    ERROR_INSTRUCTION,     /**< Invalid instruction or operand */
    ERROR_GENERAL          /**< Miscellaneous/general error */
} ErrorType;

/*-----------------------------------------------------------------------------
  Error Reporting API
  ---------------------------------------------------------------------------*/

/**
 * @brief Print formatted error message to stderr
 *
 * Includes optional context: source file name and line number.
 *
 * @param type Error category
 * @param format printf-style message format
 * @param ... Format arguments
 */
void report_error(ErrorType type, const char *format, ...);

/**
 * @brief Set current source file for contextual errors
 *
 * @param filename Input filename
 */
void set_current_file(const char *filename);

/**
 * @brief Set current source line number for contextual errors
 *
 * @param line Line number
 */
void set_current_line(int line);

#endif /* ERRORS_H */
