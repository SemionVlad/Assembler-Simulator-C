/**
 * @file preproc.h
 * @brief Preprocessor Interface for Assembly Source Processing
 *
 * Provides preprocessing utilities for the assembler, including:
 * - Macro expansion
 * - Comment removal
 * - Whitespace normalization
 * - Syntax checking
 * 
 * Also contains fallback definitions like `MachineWord` due to removal of
 * machine_word.h.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A
 */

#ifndef PREPROC_H
#define PREPROC_H

#include <stdio.h>
#include "globals.h"
#include "macro.h"
#include "cpu.h"

/*--------------------------------------------------------
  Status Codes for Preprocessor Stage
  --------------------------------------------------------*/

/**
 * @enum PreprocessorStatus
 * @brief Indicates success/failure of preprocessor actions
 */
typedef enum {
    PREPROC_SUCCESS,           /**< Operation completed successfully */
    PREPROC_ERROR_INPUT,       /**< Input file read error */
    PREPROC_ERROR_OUTPUT,      /**< Output file write error */
    PREPROC_ERROR_MEMORY,      /**< Memory allocation error */
    PREPROC_ERROR_MACRO,       /**< Macro processing failure */
    PREPROC_ERROR_SYNTAX,      /**< Invalid assembly syntax */
    PREPROC_ERROR_LINE_LENGTH  /**< Line exceeds max length */
} PreprocessorStatus;



/*--------------------------------------------------------
  Preprocessor State Tracking
  --------------------------------------------------------*/

/**
 * @struct PreprocessorState
 * @brief Tracks file-level and macro context during preprocessing
 */
typedef struct {
    MacroTable macro_table;    /**< Macro table for expansion */
    int line_number;           /**< Current line number */
    char *current_file;        /**< Current input file name */
    int error_count;           /**< Accumulated errors */
} PreprocessorState;

/*--------------------------------------------------------
  Preprocessing Core API
  --------------------------------------------------------*/

/**
 * @brief Initialize preprocessor state
 * 
 * Sets default values and initializes macro table.
 * 
 * @param state Pointer to PreprocessorState
 * @return PREPROC_SUCCESS on success, PREPROC_ERROR_MEMORY on failure
 */
PreprocessorStatus init_preprocessor(PreprocessorState *state);

/**
 * @brief Free preprocessor state
 * 
 * Releases allocated memory and internal macro table resources.
 * 
 * @param state Pointer to PreprocessorState
 */
void free_preprocessor(PreprocessorState *state);

/**
 * @brief Preprocess source file
 * 
 * Opens input file, creates an output path in output_files/am/, 
 * runs macro expansion, and writes results.
 * 
 * @param input_file Source file with .as extension
 * @return PreprocessorStatus status code
 */
PreprocessorStatus preprocess_file(const char *input_file);

/*--------------------------------------------------------
  Utility and Validation
  --------------------------------------------------------*/

/**
 * @brief Return human-readable error string
 * 
 * Provides a printable message corresponding to a given error code.
 * 
 * @param status Status code from PreprocessorStatus enum
 * @return const char* Static string describing the error
 */
const char *get_preprocessor_error(PreprocessorStatus status);

#endif /* PREPROC_H */ 
