/**
 * @file errors.c
 * @brief Error Reporting Module
 *
 * Implements contextual and formatted error reporting
 * for the assembler, including file/line annotations.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "errors.h"

/*-----------------------------------------------------------------------------
  Static State Variables
  ---------------------------------------------------------------------------*/

/**
 * @brief Current file name used in error messages (set externally)
 */
static const char *current_file = NULL;

/**
 * @brief Current line number used in error messages (set externally)
 */
static int current_line = 0;

/*-----------------------------------------------------------------------------
  Internal Utility Functions
  ---------------------------------------------------------------------------*/

/**
 * @brief Convert an error type enum to human-readable label
 *
 * @param type Error classification
 * @return const char* Corresponding label
 */
static const char *get_error_type_label(ErrorType type) {
    switch (type) {
        case ERROR_NONE:        return "None";
        case ERROR_FILE:        return "File";
        case ERROR_MEMORY:      return "Memory";
        case ERROR_SYNTAX:      return "Syntax";
        case ERROR_RANGE:       return "Range";
        case ERROR_SYMBOL:      return "Symbol";
        case ERROR_DIRECTIVE:   return "Directive";
        case ERROR_MACRO:       return "Macro";
        case ERROR_INSTRUCTION: return "Instruction";
        case ERROR_GENERAL:     return "General";
        default:                return "Unknown";
    }
}

/*-----------------------------------------------------------------------------
  Public Interface Implementation
  ---------------------------------------------------------------------------*/

/**
 * @brief Set the current file context for error messages
 *
 * @param filename Current source filename
 */
void set_current_file(const char *filename) {
    current_file = filename;
}

/**
 * @brief Set the current line context for error messages
 *
 * @param line Current line number
 */
void set_current_line(int line) {
    current_line = line;
}

/**
 * @brief Print formatted error message to stderr
 *
 * Includes optional file and line number context.
 *
 * @param type Error type (classification)
 * @param format printf-style format string
 * @param ... Variable arguments
 */
void report_error(ErrorType type, const char *format, ...) {
    va_list args;

    fprintf(stderr, "[Error - %s]", get_error_type_label(type));

    if (current_file != NULL) {
        fprintf(stderr, " in file \"%s\"", current_file);
    }

    if (current_line > 0) {
        fprintf(stderr, " at line %d", current_line);
    }

    fprintf(stderr, ": ");

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "\n");
}
