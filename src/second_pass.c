/**
 * @file second_pass.c
 * @brief Second Pass Implementation
 *
 * Resolves symbol addresses and encodes the final machine words.
 * Writes outputs into .ob, .ent, and .ext files.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Course: System Programming Laboratory (20465)
 * Version: 2025A
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "second_pass.h"
#include "symbols.h"
#include "errors.h"
#include "globals.h"
#include "utils.h"
#include "text_parser.h"
#include "cpu.h"

/**
 * @brief Executes the second pass of the assembler.
 *
 * This function re-parses the preprocessed `.am` file, handles `.entry` directives,
 * and prepares data structures for final output. Instruction operands and external
 * symbols are resolved (basic implementation placeholder).
 *
 * @param filename The input `.am` file
 * @param state Pointer to AssemblerState (shared across passes)
 * @return int 1 if successful, 0 on failure
 */
int run_second_pass(const char *filename, AssemblerState *state) {
    FILE *fp;
    char line[MAX_LINE_LENGTH + 2];
    char *label;
    char *directive;
    char *args;
    int line_number = 0;
    int pos;

    if (!filename || !state) return 0;

    fp = fopen(filename, "r");
    if (!fp) {
        report_error(ERROR_FILE, "Cannot open file for second pass: %s", filename);
        return 0;
    }

    while (fgets(line, sizeof(line), fp)) {
        pos = 0;
        label = directive = args = NULL;

        line_number++;
        set_current_line(line_number);
        set_current_file(filename);

        normalize_string(line, 1);
        skip_whitespace(line, &pos);

        if (line[pos] == '\0' || line[pos] == COMMENT_CHAR) continue;

        label = extract_label(line, &pos);
        directive = extract_directive(line, &pos);

        if (directive) {
            args = extract_arguments(line, &pos);

            if (strcmp(directive, ENTRY_DIRECTIVE) == 0) {
                if (!mark_entry_symbol(args)) {
                    report_error(ERROR_SYMBOL, "Failed to mark symbol as entry: %s", args);
                }
            }

            if (args) free(args);
            free(directive);
        } else {
            /* Placeholder: second-pass operand resolution logic */
            state->instruction_counter += 2;
        }

        if (label) free(label);
    }

    fclose(fp);
    return 1;
}

/**
 * @brief Generates output files: .ob (object), .ent (entries), and .ext (externals).
 *
 * Writes machine code to the .ob file, entry symbols to the .ent file,
 * and prepares for future support of external symbols in the .ext file.
 *
 * @param source_file The original source filename to derive output paths from
 * @param state Pointer to assembler state containing code/data images
 * @return int 1 if all files written successfully, 0 on failure
 */
int generate_output_files(const char *source_file, const AssemblerState *state) {
    FILE *ob, *ent, *ext;
    int total, i;
    char *ob_file, *ent_file, *ext_file;

    ob_file = create_output_path(source_file, "ob", ".ob");
    ent_file = create_output_path(source_file, "ent", ".ent");
    ext_file = create_output_path(source_file, "ext", ".ext");

    ob = fopen(ob_file, "w");
    if (!ob) {
        report_error(ERROR_FILE, "Cannot write to object file: %s", ob_file);
        free(ob_file);
        free(ent_file);
        free(ext_file);
        return 0;
    }

    /* Write header: code + data size */
    fprintf(ob, "%d %d\n", state->instruction_counter, state->data_counter);

    /* Write code section */
    for (i = 0; i < state->instruction_counter; i++) {
        fprintf(ob, "%04d %06X\n", i + START_ADDRESS, get_full_word_value(&state->code_image[i]));
    }

    /* Write data section */
    for (i = 0; i < state->data_counter; i++) {
        fprintf(ob, "%04d %06X\n", state->instruction_counter + START_ADDRESS + i, get_full_word_value(&state->data_image[i]));
    }

    fclose(ob);

    /* Write entry symbols */
    ent = fopen(ent_file, "w");
    if (ent) {
        total = get_symbol_table_size();
        for (i = 0; i < total; i++) {
            if (is_entry_symbol(i)) {
                fprintf(ent, "%s %04d\n", get_symbol_name(i), get_symbol_value_by_index(i));
            }
        }
        fclose(ent);
    }

    /* Placeholder for externals */
    ext = fopen(ext_file, "w");
    if (ext) {
        /* Future: externals implementation */
        fclose(ext);
    }

    free(ob_file);
    free(ent_file);
    free(ext_file);

    return 1;
} 
