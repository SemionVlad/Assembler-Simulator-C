/**
 * @file second_pass.h
 * @brief Second Pass Header
 *
 * Declares functions used during the second pass of the assembler process.
 * Responsibilities:
 * - Resolve symbols
 * - Finalize machine code encoding
 * - Generate output files (.ob, .ent, .ext)
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Course: System Programming Laboratory (20465)
 * Version: 2025A
 */

#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "globals.h"
#include "first_pass.h"
#include "cpu.h"

/**
 * @brief Perform the second pass of the assembler
 *
 * Reads .am file, resolves symbols, completes encoding,
 * and prepares the memory image for output.
 *
 * @param filename Preprocessed input file (.am)
 * @param state Pointer to initialized AssemblerState from first pass
 * @return int 1 on success, 0 on error
 */
int run_second_pass(const char *filename, AssemblerState *state);

/**
 * @brief Generate all final output files in output folders
 *
 * Produces:
 * - Object file (.ob) to output_files/ob/
 * - Entry file (.ent) to output_files/ent/
 * - External references file (.ext) to output_files/ext/
 *
 * @param source_file Source .as or .am file used to derive output filenames
 * @param state Final assembler state
 * @return int 1 on success, 0 on failure
 */
int generate_output_files(const char *source_file, const AssemblerState *state);

#endif /* SECOND_PASS_H */
