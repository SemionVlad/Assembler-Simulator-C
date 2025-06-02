/**
 * @file symbols.c
 * @brief Symbol Table Implementation for the Two-Pass Assembler
 *
 * Implements symbol storage, retrieval, marking, and validation.
 * The symbol table supports entries for labels, data, externs,
 * and adjusts data label addresses after the first pass.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbols.h"
#include "errors.h"
#include "globals.h"
#include "utils.h"

/* ----------------------------
   Symbol Table Configuration
   ---------------------------- */
#define MAX_SYMBOLS 1000 /**< Maximum number of symbols allowed */

static char *symbol_names[MAX_SYMBOLS];           /**< Symbol names */
static int symbol_values[MAX_SYMBOLS];            /**< Symbol values (addresses or data) */
static int symbol_types[MAX_SYMBOLS];             /**< Symbol classification (code/data/etc.) */
static int symbol_entry_flags[MAX_SYMBOLS];       /**< Flags to mark entry points */
static int symbol_count = 0;                      /**< Number of stored symbols */

/*-----------------------------------------------
  Symbol Table API
  -----------------------------------------------*/
  
/**
 * @brief Initialize the symbol table
 *
 * Resets the symbol table for a new assembly file.
 *
 * @return int 1 on success
 */
int init_symbol_table(void) {
    symbol_count = 0;  /* Reset symbol counter */
    return 1;
}

/**
 * @brief Add a symbol to the table
 *
 * Validates uniqueness and stores name, value, and type.
 *
 * @param name Symbol name
 * @param value Memory address or data value
 * @param type Symbol type (SYMBOL_CODE, SYMBOL_DATA, etc.)
 * @return int 1 if added successfully, 0 otherwise
 */
int add_symbol(const char *name, int value, int type) {
    int i;

    /* Check if table is full */
    if (symbol_count >= MAX_SYMBOLS) {
        report_error(ERROR_SYMBOL, "Symbol table full");
        return 0;
    }

    /* Validate uniqueness */
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_names[i], name) == 0) {
            report_error(ERROR_SYMBOL, "Symbol already exists: %s", name);
            return 0;
        }
    }

    /* Store symbol info */
    symbol_names[symbol_count] = safe_strdup(name);
    symbol_values[symbol_count] = value;
    symbol_types[symbol_count] = type;
    symbol_entry_flags[symbol_count] = (type == SYMBOL_ENTRY); /* Initially mark if entry */
    symbol_count++;
    return 1;
}

/**
 * @brief Retrieve the value of a symbol by name
 *
 * @param name Symbol name to look up
 * @return int Symbol value or -1 if not found
 */
int get_symbol_value(const char *name) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_names[i], name) == 0) {
            return symbol_values[i];
        }
    }
    return -1;  /* Not found */
}

/**
 * @brief Update a symbol's value
 *
 * Used to update addresses after first pass.
 *
 * @param name Symbol name
 * @param new_value New memory address
 * @return int 1 if updated successfully, 0 otherwise
 */
int update_symbol_value(const char *name, int new_value) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_names[i], name) == 0) {
            symbol_values[i] = new_value;
            return 1;
        }
    }
    report_error(ERROR_SYMBOL, "Symbol not found: %s", name);
    return 0;
}

/**
 * @brief Mark a symbol as entry
 *
 * Entry symbols are later written to the .ent file.
 *
 * @param name Symbol name to mark
 * @return int 1 if marked successfully, 0 if not found or extern
 */
int mark_entry_symbol(const char *name) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_names[i], name) == 0) {
            if (symbol_types[i] == SYMBOL_EXTERN) {
                report_error(ERROR_SYMBOL, "Cannot mark extern as entry: %s", name);
                return 0;
            }
            symbol_entry_flags[i] = 1;
            return 1;
        }
    }
    report_error(ERROR_SYMBOL, "Symbol not found: %s", name);
    return 0;
}

/**
 * @brief Adjust addresses of data symbols after first pass
 *
 * Adds the instruction counter to all data symbol values.
 *
 * @param ic Instruction counter to add
 */
void adjust_data_symbol_addresses(int ic) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (symbol_types[i] == SYMBOL_DATA) {
            symbol_values[i] += ic;  /* Offset data symbol addresses */
        }
    }
}

/**
 * @brief Validate that entry and extern symbols are not the same
 *
 * Ensures logical consistency in the symbol table.
 *
 * @return int 1 if table is valid, 0 otherwise
 */
int validate_symbol_table(void) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (symbol_types[i] == SYMBOL_EXTERN && symbol_entry_flags[i]) {
            report_error(ERROR_SYMBOL, "Symbol cannot be both extern and entry: %s", symbol_names[i]);
            return 0;
        }
    }
    return 1;
}


/**
 * @brief Free all memory used by the symbol table
 *
 * Clears dynamically allocated symbol names.
 */
void free_symbol_table(void) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        free(symbol_names[i]);
    }
    symbol_count = 0;
}

/*-----------------------------------------------
  Symbol Table Read-Only Accessors
  -----------------------------------------------*/

/**
 * @brief Get total number of symbols in table
 * 
 * @return int Symbol count
 */
int get_symbol_table_size(void) {
    return symbol_count;
}

/**
 * @brief Get symbol name by index
 *
 * @param index Index in symbol table
 * @return const char* Symbol name (read-only)
 */
const char *get_symbol_name(int index) {
    return symbol_names[index];
}

/**
 * @brief Get symbol value by index
 *
 * @param index Index in symbol table
 * @return int Symbol value
 */
int get_symbol_value_by_index(int index) {
    return symbol_values[index];
}

/**
 * @brief Check if symbol at index is marked as entry
 *
 * @param index Index in symbol table
 * @return int 1 if entry, 0 otherwise
 */
int is_entry_symbol(int index) {
    return symbol_entry_flags[index];
}
