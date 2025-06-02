/**
 * @file first_pass.c
 * @brief First pass implementation for assembler
 *
 * Parses each line of the preprocessed file (.am), processes labels and directives,
 * builds the symbol table, and populates data/code images.
 * This pass does not resolve symbol references—it only collects information.
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

#include "first_pass.h"
#include "symbols.h"
#include "errors.h"
#include "globals.h"
#include "utils.h"
#include "text_parser.h"
#include "cpu.h"

/**
 * @brief Initialize the assembler state for a new run.
 *
 * Allocates space for code and data images, and resets counters.
 *
 * @param state Pointer to AssemblerState structure to initialize
 */
void init_assembler_state(AssemblerState *state) {
    state->code_image = malloc(sizeof(MachineWord) * MAX_DATA_VALUES);  /* Allocate code image array */
    state->code_capacity = MAX_DATA_VALUES;
    state->code_size = 0;

    state->data_image = malloc(sizeof(MachineWord) * MAX_DATA_VALUES);  /* Allocate data image array */
    state->data_capacity = MAX_DATA_VALUES;
    state->data_size = 0;

    state->instruction_counter = 0;
    state->data_counter = 0;
    state->error_count = 0;
}

/**
 * @brief Free memory allocated within the assembler state.
 *
 * Releases code and data image memory and resets pointers to NULL.
 *
 * @param state Pointer to AssemblerState structure to free
 */
void free_assembler_state(AssemblerState *state) {
    if (state->code_image) {
        free(state->code_image);      /* Release code image */
        state->code_image = NULL;
    }
    if (state->data_image) {
        free(state->data_image);      /* Release data image */
        state->data_image = NULL;
    }
}

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
int run_first_pass(const char *filename, AssemblerState *state) {
    FILE *fp;
    char line[MAX_LINE_LENGTH + 2];
    int line_number = 0;
    int success = 1;
        

    if (!filename || !state) return 0;

    fp = fopen(filename, "r");
    if (!fp) {
        report_error(ERROR_FILE, "Cannot open file for first pass: %s", filename);
        return 0;
    }

    while (fgets(line, sizeof(line), fp)) {
        int pos = 0;
        char *label = NULL;
        char *directive = NULL;
        char *args = NULL;
        int i, count = 0, length = 0;
        int values[MAX_DATA_VALUES];
        int chars[MAX_STRING_LENGTH];

        /* Update line and context for error reporting */
        line_number++;
        set_current_line(line_number);
        set_current_file(filename);

        /* Normalize and clean the line */
        normalize_string(line, 1);
        remove_comment(line);
        skip_whitespace(line, &pos);

        /* Skip empty or comment-only lines */
        if (line[pos] == '\0' || line[pos] == '\n') continue;

        /* Extract label and directive */
        label = extract_label(line, &pos);
        directive = extract_directive(line, &pos);

        if (directive) {
            args = extract_arguments(line, &pos);

            /* Process .data directive */
            if (strcmp(directive, DATA_DIRECTIVE) == 0) {
                count = parse_data_values(args, values, MAX_DATA_VALUES);
                if (count < 0) {
                    success = 0;
                    continue;
                }
                if (label) {
                    add_symbol(label, state->data_counter + START_ADDRESS, SYMBOL_DATA);
                }
                for (i = 0; i < count; i++) {
                    init_machine_word(&state->data_image[state->data_counter++], values[i], ARE_ABSOLUTE);
                }
            }
            /* Process .string directive */
            else if (strcmp(directive, STRING_DIRECTIVE) == 0) {
                length = parse_string_value(args, chars, MAX_STRING_LENGTH);
                if (length < 0) {
                    success = 0;
                    continue;
                }
                if (label) {
                    add_symbol(label, state->data_counter + START_ADDRESS, SYMBOL_DATA);
                }
                for (i = 0; i < length; i++) {
                    init_machine_word(&state->data_image[state->data_counter++], chars[i], ARE_ABSOLUTE);
                }
            }
            /* Process extern/entry (entry handled later) */
            else if (strcmp(directive, ENTRY_DIRECTIVE) == 0 ||
                         strcmp(directive, EXTERN_DIRECTIVE) == 0) {
                if (strcmp(directive, EXTERN_DIRECTIVE) == 0 && args) {
                        add_symbol(args, 0, SYMBOL_EXTERN);
                }
            }
            /* Unknown directive encountered */
            else {
                report_error(ERROR_SYNTAX, "Unknown directive: %s", directive);
                success = 0;
            }

            /* Clean allocated memory */
            free(directive);
            if (args) free(args);
        } else {
            /* Instruction line: if label exists, store it */
            if (label) {
                add_symbol(label, state->instruction_counter + START_ADDRESS, SYMBOL_CODE);
            }

            /* Assume each instruction takes 2 words */
            state->instruction_counter += 2;
        }

        if (label) free(label);
    }

    fclose(fp);

    /* Adjust data symbol addresses (added after code section) */
    adjust_data_symbol_addresses(state->instruction_counter);

    /* Final symbol table validation (entry vs extern) */
    if (!validate_symbol_table()) success = 0;

    return success;
} 
