/**
 * @file macro.h
 * @brief Macro Handling and Expansion
 *
 * Manages macro detection, storage, and expansion in the assembler preprocessor.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A
 */

#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>

#include "globals.h"

/*---------------------------------------------
  Constants
  ---------------------------------------------*/

#define MAX_MACROS 100             /**< Maximum number of macros */
#define MAX_MACRO_NAME 31          /**< Maximum macro name length */
#define MAX_MACRO_LINES 100        /**< Maximum number of lines per macro */

/*---------------------------------------------
  Macro Status Codes
  ---------------------------------------------*/

/**
 * @enum MacroStatus
 * @brief Status codes returned from macro functions.
 */
typedef enum {
    MACRO_SUCCESS,         /**< Operation successful */
    MACRO_ERROR_NAME,      /**< Invalid macro name */
    MACRO_ERROR_MEMORY,    /**< Memory allocation error */
    MACRO_ERROR_SYNTAX,    /**< Syntax error */
    MACRO_ERROR_DUPLICATE, /**< Duplicate macro definition */
    MACRO_ERROR_LIMIT,     /**< Exceeded macro limits */
    MACRO_ERROR_NESTING,   /**< Nested macro definitions not allowed */
    MACRO_ERROR_IO         /**< I/O error occurred */
} MacroStatus;

/*---------------------------------------------
  Data Structures
  ---------------------------------------------*/

/**
 * @struct Macro
 * @brief Represents a single macro definition.
 */
typedef struct {
    char name[MAX_MACRO_NAME + 1]; /**< Name of the macro */
    char **lines;                  /**< Array of strings holding macro content */
    int line_count;                /**< Number of lines in the macro */
} Macro;

/**
 * @struct MacroTable
 * @brief Holds all defined macros during preprocessing.
 */
typedef struct {
    Macro macros[MAX_MACROS]; /**< Array of macro definitions */
    int count;                /**< Current number of stored macros */
} MacroTable;

/*---------------------------------------------
  Core Macro Table Management
  ---------------------------------------------*/

/**
 * @brief Initialize the macro table.
 *
 * Sets all values in the macro table to default.
 *
 * @param table Pointer to macro table to initialize
 * @return MacroStatus Status code
 */
MacroStatus init_macro_table(MacroTable *table);

/**
 * @brief Free all memory used in macro table.
 *
 * Clears all allocated lines and resets the table.
 *
 * @param table Pointer to macro table
 */
void free_macro_table(MacroTable *table);

/**
 * @brief Add a new macro to the table.
 *
 * Validates the name and initializes line storage.
 *
 * @param table Macro table to update
 * @param name Name of the macro
 * @return Macro* Pointer to created macro, or NULL on error
 */
Macro *add_macro(MacroTable *table, const char *name);

/**
 * @brief Append a new line to an existing macro.
 *
 * Dynamically duplicates and stores the given line.
 *
 * @param macro Target macro
 * @param line Line content to append
 * @return MacroStatus Result code
 */
MacroStatus add_macro_line(Macro *macro, const char *line);

/**
 * @brief Find macro by name.
 *
 * Searches the macro table linearly for a given name.
 *
 * @param table Macro table to search
 * @param name Macro name to search
 * @return Macro* Pointer to matching macro or NULL
 */
Macro *find_macro(const MacroTable *table, const char *name);

/**
 * @brief Expand macros into their content during preprocessing.
 *
 * Handles detection, storing, and substitution of macros in input.
 *
 * @param input Input source file (original .as)
 * @param output Output preprocessed file (.am)
 * @param table Macro table used during expansion
 * @return MacroStatus Result code
 */
MacroStatus expand_macros(FILE *input, FILE *output, MacroTable *table);

/*---------------------------------------------
  Macro Syntax & Name Validation
  ---------------------------------------------*/

/**
 * @brief Check if a line begins a macro definition.
 *
 * Recognizes lines that start with "mcro".
 *
 * @param line Input line
 * @return int 1 if true, 0 if not
 */
int is_macro_definition(const char *line);

/**
 * @brief Check if a line ends a macro definition.
 *
 * Recognizes lines that contain only "endmcro".
 *
 * @param line Input line
 * @return int 1 if true, 0 if not
 */
int is_macro_end(const char *line);

/**
 * @brief Validate a macro name.
 *
 * Ensures name contains only alphanumeric and underscore,
 * and doesn't conflict with reserved keywords.
 *
 * @param name Name to validate
 * @return int 1 if valid, 0 if not
 */
int is_valid_macro_name(const char *name);

/**
 * @brief Parse a macro definition line to extract the name.
 *
 * Validates and extracts name following "mcro" directive.
 *
 * @param line Line to parse
 * @param name Output buffer for name
 * @param name_size Size of the name buffer
 * @return MacroStatus Parsing result
 */
MacroStatus parse_macro_definition(const char *line, char *name, size_t name_size);

/*---------------------------------------------
  Error String Representation
  ---------------------------------------------*/

/**
 * @brief Convert MacroStatus code to human-readable message.
 *
 * @param status Status code
 * @return const char* Message string
 */
const char *get_macro_error(MacroStatus status);

#endif /* MACRO_H */ 
