/**
 * @file preproc.c
 * @brief Assembly Source Preprocessor Implementation
 *
 * Handles preprocessing steps including:
 * - Macro expansion
 * - Comment removal
 * - Whitespace normalization
 * - Syntax validation
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Course: System Programming Laboratory (20465)
 * Version: 2025A
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "preproc.h"
#include "errors.h"
#include "utils.h"

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
PreprocessorStatus init_preprocessor(PreprocessorState *state) {
    if (!state) return PREPROC_ERROR_MEMORY;

    /* Initialize state fields */
    state->line_number = 0;
    state->current_file = NULL;
    state->error_count = 0;

    /* Initialize macro table */
    return init_macro_table(&state->macro_table) == MACRO_SUCCESS
           ? PREPROC_SUCCESS
           : PREPROC_ERROR_MEMORY;
}

/**
 * @brief Free preprocessor state
 * 
 * Releases allocated memory and internal macro table resources.
 * 
 * @param state Pointer to PreprocessorState
 */
void free_preprocessor(PreprocessorState *state) {
    if (state) {
        if (state->current_file)
            free(state->current_file); /* Free current file path string */
        
        free_macro_table(&state->macro_table); /* Free macro hash table */
    }
}

/**
 * @brief Preprocess source file
 * 
 * Opens input file, creates an output path in output_files/am/, 
 * runs macro expansion, and writes results.
 * 
 * @param input_file Source file with .as extension
 * @return PreprocessorStatus status code
 */
PreprocessorStatus preprocess_file(const char *input_file) {
    FILE *input = NULL, *output = NULL;
    PreprocessorState state;
    PreprocessorStatus result = PREPROC_SUCCESS;
    char *output_file = NULL;

    /* Initialize preprocessing environment and macro table */
    if (init_preprocessor(&state) != PREPROC_SUCCESS)
        return PREPROC_ERROR_MEMORY;

    /* Duplicate input filename for state tracking */
    state.current_file = safe_strdup(input_file);
    if (!state.current_file)
        return PREPROC_ERROR_MEMORY;

    /* Open source file for reading */
    input = fopen(input_file, "r");
    if (!input)
        return PREPROC_ERROR_INPUT;

    /* Create output path automatically inside output_files/am/ */
    output_file = create_output_path(input_file, "am", ".am");
    if (!output_file) {
        fclose(input);
        return PREPROC_ERROR_OUTPUT;
    }

    /* Open destination file for writing */
    output = fopen(output_file, "w");
    if (!output) {
        fclose(input);
        free(output_file);
        return PREPROC_ERROR_OUTPUT;
    }

    /* Perform macro expansion and write result to output */
    result = expand_macros(input, output, &state.macro_table);

    /* Clean up resources */
    fclose(input);
    fclose(output);
    free(output_file);
    free_preprocessor(&state);

    return result;
}

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
const char *get_preprocessor_error(PreprocessorStatus status) {
    switch (status) {
        case PREPROC_SUCCESS: return "Success";
        case PREPROC_ERROR_INPUT: return "Error reading input file";
        case PREPROC_ERROR_OUTPUT: return "Error writing output file";
        case PREPROC_ERROR_MEMORY: return "Memory allocation failed";
        case PREPROC_ERROR_MACRO: return "Macro expansion error";
        case PREPROC_ERROR_SYNTAX: return "Syntax error";
        case PREPROC_ERROR_LINE_LENGTH: return "Line length exceeded";
        default: return "Unknown preprocessor error";
    }
}
