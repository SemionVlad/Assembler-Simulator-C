/**
 * @file symbols.h
 * @brief Symbol Table Interface
 *
 * This module handles symbol definitions and lookup for the assembler.
 * Includes support for data, code, external, and entry symbols.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdio.h>

/*-----------------------------------------------
  Symbol Type Constants
  -----------------------------------------------*/

#define SYMBOL_CODE    0 /**< Symbol for code section */
#define SYMBOL_DATA    1 /**< Symbol for data section */
#define SYMBOL_EXTERN  2 /**< Symbol declared as external */
#define SYMBOL_ENTRY   3 /**< Symbol declared as entry */

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
int init_symbol_table(void);

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
int add_symbol(const char *name, int value, int type);

/**
 * @brief Retrieve the value of a symbol by name
 *
 * @param name Symbol name to look up
 * @return int Symbol value or -1 if not found
 */
int get_symbol_value(const char *name);

/**
 * @brief Update a symbol's value
 *
 * Used to update addresses after first pass.
 *
 * @param name Symbol name
 * @param new_value New memory address
 * @return int 1 if updated successfully, 0 otherwise
 */
int update_symbol_value(const char *name, int new_value);

/**
 * @brief Mark a symbol as entry
 *
 * Entry symbols are later written to the .ent file.
 *
 * @param name Symbol name to mark
 * @return int 1 if marked successfully, 0 if not found or extern
 */
int mark_entry_symbol(const char *name);

/**
 * @brief Adjust addresses of data symbols after first pass
 *
 * Adds the instruction counter to all data symbol values.
 *
 * @param ic Instruction counter to add
 */
void adjust_data_symbol_addresses(int ic);

/**
 * @brief Validate that entry and extern symbols are not the same
 *
 * Ensures logical consistency in the symbol table.
 *
 * @return int 1 if table is valid, 0 otherwise
 */
int validate_symbol_table(void);

/**
 * @brief Free all memory used by the symbol table
 *
 * Clears dynamically allocated symbol names.
 */
void free_symbol_table(void);

/*-----------------------------------------------
  Symbol Table Read-Only Accessors
  -----------------------------------------------*/

/**
 * @brief Get total number of symbols in table
 * @return int Symbol count
 */
int get_symbol_table_size(void);

/**
 * @brief Get symbol name by index
 *
 * @param index Index in symbol table
 * @return const char* Symbol name (read-only)
 */
const char *get_symbol_name(int index);

/**
 * @brief Get symbol value by index
 *
 * @param index Index in symbol table
 * @return int Symbol value
 */
int get_symbol_value_by_index(int index);

/**
 * @brief Check if symbol at index is marked as entry
 *
 * @param index Index in symbol table
 * @return int 1 if entry, 0 otherwise
 */
int is_entry_symbol(int index);

#endif /* SYMBOLS_H */
