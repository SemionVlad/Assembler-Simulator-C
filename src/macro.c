/**
 * @file macro.c
 * @brief Macro Processing and Expansion Module
 *
 * Implements macro definition storage, expansion, and syntax validation.
 * Handles multi-line macro definitions, invocation replacement, and nesting detection.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "macro.h"
#include "utils.h"
#include "errors.h"
#include "globals.h"

/**
 * @brief Initialize the macro table.
 *
 * Sets all values in the macro table to default.
 *
 * @param table Pointer to macro table to initialize
 * @return MacroStatus Status code
 */
MacroStatus init_macro_table(MacroTable *table) {
    if (!table) return MACRO_ERROR_MEMORY;
    table->count = 0;
    return MACRO_SUCCESS;
}

/**
 * @brief Free all memory used in macro table.
 *
 * Clears all allocated lines and resets the table.
 *
 * @param table Pointer to macro table
 */
void free_macro_table(MacroTable *table) {
    int i, j;
    for (i = 0; i < table->count; i++) {
        for (j = 0; j < table->macros[i].line_count; j++) {
            free(table->macros[i].lines[j]);
        }
        free(table->macros[i].lines);
    }
    table->count = 0;
}

/**
 * @brief Add a new macro to the table.
 *
 * Validates the name and initializes line storage.
 *
 * @param table Macro table to update
 * @param name Name of the macro
 * @return Macro* Pointer to created macro, or NULL on error
 */
Macro *add_macro(MacroTable *table, const char *name) {
    Macro *m;

    if (!table || !name) return NULL;
    if (!is_valid_macro_name(name)) return NULL;
    if (find_macro(table, name)) return NULL;
    if (table->count >= MAX_MACROS) return NULL;

    m = &table->macros[table->count++];
    strncpy(m->name, name, MAX_MACRO_NAME);
    m->name[MAX_MACRO_NAME] = '\0';
    m->lines = calloc(MAX_MACRO_LINES, sizeof(char *));
    m->line_count = 0;
    return m;
}

/**
 * @brief Append a new line to an existing macro.
 *
 * Dynamically duplicates and stores the given line.
 *
 * @param macro Target macro
 * @param line Line content to append
 * @return MacroStatus Result code
 */
MacroStatus add_macro_line(Macro *macro, const char *line) {
    if (!macro || !line) return MACRO_ERROR_SYNTAX;
    if (macro->line_count >= MAX_MACRO_LINES) return MACRO_ERROR_LIMIT;

    macro->lines[macro->line_count] = safe_strdup(line);
    macro->line_count++;
    return MACRO_SUCCESS;
}

/**
 * @brief Find macro by name.
 *
 * Searches the macro table linearly for a given name.
 *
 * @param table Macro table to search
 * @param name Macro name to search
 * @return Macro* Pointer to matching macro or NULL
 */
Macro *find_macro(const MacroTable *table, const char *name) {
    int i;
    for (i = 0; i < table->count; i++) {
        if (strcmp(table->macros[i].name, name) == 0) {
            return (Macro *)&table->macros[i];
        }
    }
    return NULL;
}

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
MacroStatus expand_macros(FILE *input, FILE *output, MacroTable *table) {
    char line[MAX_LINE_LENGTH + 2];
    char macro_name[MAX_MACRO_NAME + 1];
    Macro *current_macro = NULL;
    int i, j, written;

    while (fgets(line, sizeof(line), input)) {
        char *normalized = safe_strdup(line);
        if (!normalized) return MACRO_ERROR_MEMORY;

        normalize_string(normalized, 1);

        if (is_macro_definition(normalized)) {
            if (current_macro) {
                report_error(ERROR_SYNTAX, "Nested macro definition");
                free(normalized);
                return MACRO_ERROR_NESTING;
            }

            if (parse_macro_definition(normalized, macro_name, sizeof(macro_name)) != MACRO_SUCCESS) {
                free(normalized);
                return MACRO_ERROR_NAME;
            }

            current_macro = add_macro(table, macro_name);
            if (!current_macro) {
                free(normalized);
                return MACRO_ERROR_MEMORY;
            }
        } else if (is_macro_end(normalized)) {
            if (!current_macro) {
                report_error(ERROR_SYNTAX, "Unexpected macro end");
                free(normalized);
                return MACRO_ERROR_SYNTAX;
            }
            current_macro = NULL;
        } else if (current_macro) {
            if (add_macro_line(current_macro, normalized) != MACRO_SUCCESS) {
                free(normalized);
                return MACRO_ERROR_MEMORY;
            }
        } else {
            written = 0;
            for (i = 0; i < table->count; i++) {
                if (strcmp(normalized, table->macros[i].name) == 0) {
                    for (j = 0; j < table->macros[i].line_count; j++) {
                        fprintf(output, "%s\n", table->macros[i].lines[j]);
                    }
                    written = 1;
                    break;
                }
            }
            if (!written) {
                fprintf(output, "%s", line);
            }
        }

        free(normalized);
    }

    return current_macro ? MACRO_ERROR_SYNTAX : MACRO_SUCCESS;
}

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
int is_macro_definition(const char *line) {
    while (isspace((unsigned char)*line)) line++;
    return strncmp(line, "mcro", 4) == 0;
}

/**
 * @brief Check if a line ends a macro definition.
 *
 * Recognizes lines that contain only "endmcro".
 *
 * @param line Input line
 * @return int 1 if true, 0 if not
 */
int is_macro_end(const char *line) {
    while (isspace((unsigned char)*line)) line++;
    return strncmp(line, "endmcro", 7) == 0;
}

/**
 * @brief Validate a macro name.
 *
 * Ensures name contains only alphanumeric and underscore,
 * and doesn't conflict with reserved keywords.
 *
 * @param name Name to validate
 * @return int 1 if valid, 0 if not
 */
int is_valid_macro_name(const char *name) {
    int i;
    if (!isalpha((unsigned char)name[0])) return 0;
    for (i = 1; name[i]; i++) {
        if (!isalnum((unsigned char)name[i]) && name[i] != '_') return 0;
    }
    return strlen(name) <= MAX_MACRO_NAME;
}

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
MacroStatus parse_macro_definition(const char *line, char *name, size_t name_size) {
    const char *start = line + strlen("mcro");
    size_t len;

    while (isspace((unsigned char)*start)) start++;
    len = strcspn(start, " \t\r\n");

    if (len == 0 || len >= name_size) return MACRO_ERROR_NAME;

    strncpy(name, start, len);
    name[len] = '\0';
    return MACRO_SUCCESS;
}

/*---------------------------------------------
  Error String Representation
  ---------------------------------------------*/

/**
 * @brief Convert MacroStatus code to human-readable message.
 *
 * @param status Status code
 * @return const char* Message string
 */
const char *get_macro_error(MacroStatus status) {
    switch (status) {
        case MACRO_SUCCESS:         return "No error";
        case MACRO_ERROR_NAME:      return "Invalid macro name";
        case MACRO_ERROR_MEMORY:    return "Memory error";
        case MACRO_ERROR_SYNTAX:    return "Syntax error";
        case MACRO_ERROR_DUPLICATE: return "Duplicate macro name";
        case MACRO_ERROR_LIMIT:     return "Macro line limit exceeded";
        case MACRO_ERROR_NESTING:   return "Nested macros not allowed";
        case MACRO_ERROR_IO:        return "I/O error";
        default:                    return "Unknown macro error";
    }
}
