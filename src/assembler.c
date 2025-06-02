/**
 * @file assembler.c
 * @brief Entry point for the Two-Pass Assembler
 *
 * Coordinates preprocessing, first pass, second pass, and output generation.
 * Handles input file parsing and command-line options.
 * 
 * @author Shimon Esterkin
 * @version 2025A (20465)
 * @id 207972258
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"
#include "errors.h"
#include "preproc.h"
#include "first_pass.h"
#include "second_pass.h"

/**
 * @brief Process a single assembly source file
 * 
 * Runs all compilation phases:
 * - Preprocessor (macro expansion)
 * - First pass (symbol resolution and initial encoding)
 * - Second pass (final encoding and output)
 * 
 * Generates: .am, .ob, .ent, .ext files as needed.
 * 
 * @param filename Input source filename (.as extension)
 */
void process_file(const char *filename) {
    AssemblerState state;
    char *am_file = NULL;
    int success = 1;

    printf("Processing file: %s\n", filename);

    /* Generate .am file name from input filename */
    am_file = create_output_path(filename, "am", ".am");

    /* Expand macros and write result to .am file */
    if (preprocess_file(filename) != PREPROC_SUCCESS) {
        success = 0;
        goto cleanup;
    }

    /* Initialize assembler state for the file */
    init_assembler_state(&state);

    /* First pass: collect symbols, validate syntax, encode instructions/data */
    if (!run_first_pass(am_file, &state)) {
        success = 0;
        goto cleanup_state;
    }

    /* Second pass: resolve labels and finalize instruction encoding */
    if (!run_second_pass(am_file, &state)) {
        success = 0;
        goto cleanup_state;
    }

    /* Generate output files: .ob, .ent, .ext to designated folders */
    if (!generate_output_files(filename, &state)) {
        success = 0;
    }

cleanup_state:
    /* Free dynamically allocated tables and memory from state */
    free_assembler_state(&state);

cleanup:
    /* Free allocated memory for filename strings */
    free(am_file);

    /* Print status summary */
    if (!success) {
        printf("❌ Error processing file: %s\n", filename);
    } else {
        printf("✅ Finished: %s\n", filename);
    }
}

/**
 * @brief Program entry point
 * 
 * Parses arguments and processes each input file
 * 
 * @param argc Argument count
 * @param argv Argument values
 * @return int Exit status
 */
int main(int argc, char *argv[]) {
    int i;

    /* Show banner */
    display_welcome();

    /* Require at least one input file */
    if (argc < 2) {
        fprintf(stderr, "No input files provided.\n");
        display_help();
        return EXIT_FAILURE;
    }

    /* Iterate over arguments and process each file */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            display_help();
            return EXIT_SUCCESS;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            display_version();
            return EXIT_SUCCESS;
        } else {
            /* Process current .as file */
            process_file(argv[i]);
        }
    }

    return EXIT_SUCCESS;
}
