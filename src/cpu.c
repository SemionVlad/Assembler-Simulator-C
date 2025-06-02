/**
 * @file cpu.c
 * @brief MachineWord structure logic implementation
 *
 * Handles low-level bit manipulation and machine word representation
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#include <stdio.h>
#include "cpu.h"

/**
 * @brief Initialize a MachineWord structure
 *
 * Applies bit masks to ensure valid ranges for content (21 bits) and ARE (3 bits).
 *
 * @param word Pointer to the MachineWord struct
 * @param content The binary content of the word (21 bits max)
 * @param are The A/R/E bits (3 bits max)
 */
void init_machine_word(MachineWord *word, unsigned int content, unsigned int are) {
    if (!word) return;

    /* Mask content to 21 bits: bits 0-20 */
    word->content = content & ((1 << 21) - 1);

    /* Mask ARE to 3 bits: bits 0-2 */
    word->ARE = are & 0x7;
}

/**
 * @brief Print a MachineWord in binary format to stdout
 *
 * Outputs 24 bits total: 21 for content, 3 for ARE.
 *
 * @param word Pointer to the MachineWord to print
 */
void print_machine_word(const MachineWord *word) {
    int i;
    if (!word) return;

    /* Print 21 content bits (from MSB to LSB) */
    for (i = 20; i >= 0; i--) {
        printf("%d", (word->content >> i) & 1);
    }

    /* Print 3 ARE bits (from MSB to LSB) */
    for (i = 2; i >= 0; i--) {
        printf("%d", (word->ARE >> i) & 1);
    }

    /* Newline at end of word */
    printf("\n");
}

/**
 * @brief Returns the 24-bit packed representation of a MachineWord
 *
 * Combines content and ARE into a single unsigned int:
 * content shifted left by 3, ORed with ARE bits.
 *
 * @param word Pointer to the MachineWord
 * @return unsigned int Packed 24-bit value
 */
unsigned int get_full_word_value(const MachineWord *word) {
    if (!word) return 0;

    /* Shift content and append ARE bits */
    return (word->content << 3) | (word->ARE & 0x7);
}
