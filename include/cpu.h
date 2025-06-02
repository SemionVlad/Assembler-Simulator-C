/**
 * @file cpu.h
 * @brief Core CPU word representation and low-level bitwise helpers
 *
 * This module defines the MachineWord structure used to represent
 * a single instruction or data word in the assembler. It also provides
 * helper functions for initialization, binary display, and word extraction.
 *
 * Author: Shimon Esterkin  
 * ID: 207972258  
 * Version: 2025A (20465)  
 */

#ifndef CPU_H
#define CPU_H

#include "globals.h"

/**
 * @struct MachineWord
 * @brief Represents a 24-bit machine word divided into content and ARE bits.
 *
 * The `content` field holds the instruction or data (21 bits),
 * and `ARE` holds the addressing type flags (3 bits): Absolute, Relocatable, External.
 */
typedef struct {
    unsigned int content : 21; /**< 21-bit instruction/data payload */
    unsigned int ARE : 3;      /**< 3-bit ARE field (A=4, R=2, E=1) */
} MachineWord;

/**
 * @brief Initializes a MachineWord with given values.
 *
 * Sets both the content (instruction/data) and ARE bits for a machine word.
 * 
 * @param word Pointer to the MachineWord to initialize
 * @param content The 21-bit instruction or data value
 * @param are The ARE field (must be 1, 2, or 4)
 */
void init_machine_word(MachineWord *word, unsigned int content, unsigned int are);

/**
 * @brief Prints the full 24-bit machine word in binary format.
 *
 * Used for debugging and visualizing the structure of encoded instructions.
 *
 * @param word Pointer to the MachineWord to print
 */
void print_machine_word(const MachineWord *word);

/**
 * @brief Returns the full 24-bit value of the machine word.
 *
 * Combines the 21-bit `content` and 3-bit `ARE` into a single 24-bit unsigned value.
 * 
 * @param word Pointer to MachineWord
 * @return unsigned int The 24-bit encoded value (content << 3 | ARE)
 */
unsigned int get_full_word_value(const MachineWord *word);

#endif /* CPU_H */
