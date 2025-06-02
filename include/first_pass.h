/**
 * @file first_pass.h
 * @brief First pass processing for assembler
 *
 * Handles symbol collection, address calculation, and instruction validation.
 * 
 * This module performs the **first pass** of the assembler.
 * It:
 * - Parses each line for labels, instructions, and directives
 * - Builds the symbol table
 * - Populates the code and data images partially
 * - Does not generate any output files yet
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A
 */

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "globals.h"
#include "symbols.h"
#include "code_conversion.h"
#include "errors.h"
#include "preproc.h"
#include "cpu.h"

/**
 * @struct AssemblerState
 * @brief Global state shared across both assembler passes
 */
typedef struct {
    MachineWord *code_image;
    int code_size;
    int code_capacity;
    MachineWord *data_image;
    int data_size;
    int data_capacity;
    int instruction_counter;
    int data_counter;
    int error_count;
} AssemblerState;

/**
 * @brief Initialize the assembler state for a new run.
 *
 * Allocates space for code and data images, and resets counters.
 *
 * @param state Pointer to AssemblerState structure to initialize
 */
void init_assembler_state(AssemblerState *state);

/**
 * @brief Free memory allocated within the assembler state.
 *
 * Releases code and data image memory and resets pointers to NULL.
 *
 * @param state Pointer to AssemblerState structure to free
 */
void free_assembler_state(AssemblerState *state);

/**
 * @brief Run the first pass over a preprocessed .am file.
 *
 * Processes lines to:
 * - Collect labels and build the symbol table
 * - Parse and store `.data` and `.string` content
 * - Identify `.extern` declarations
 * - Count instruction lines for code allocation
 *
 * @param filename The input file name (preprocessed .am)
 * @param state Pointer to shared assembler state structure
 * @return int 1 if successful, 0 if any error occurred
 */
int run_first_pass(const char *filename, AssemblerState *state);

#endif /* FIRST_PASS_H */ 
