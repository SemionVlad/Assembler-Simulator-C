/**
 * @file code_conversion.c
 * @brief Machine Word Encoding Implementations
 *
 * Provides functions for encoding machine words into:
 *  - Binary strings
 *  - Hexadecimal strings
 *  - Base64 strings (custom character set)
 *
 * These encodings are used in the second pass for generating the `.ob` file.
 *
 * Author: Shimon Esterkin  
 * ID: 207972258  
 * Course: System Programming Laboratory (20465)  
 * Version: 2025A  
 */

#include <stdio.h>
#include <string.h>
#include "code_conversion.h"

/* ---------------------------------------------
   Custom Base64 Encoding Table (non-standard)
   --------------------------------------------- */
static const char base64_table[64] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * @brief Converts a 24-bit word to binary string.
 *
 * Each bit is translated into '0' or '1' and written to the buffer.
 * The output buffer must be at least 25 bytes (24 bits + null).
 *
 * @param word 24-bit unsigned word
 * @param output Output buffer (25+ bytes)
 */
void word_to_binary(unsigned int word, char *output) {
    int i;
    /* Iterate from most significant bit (23) to least (0) */
    for (i = WORD_SIZE - 1; i >= 0; i--) {
        /* Shift and mask bit to extract its value */
        *output++ = ((word >> i) & 1) ? '1' : '0';
    }

    /* Null-terminate string */
    *output = '\0';
}

/**
 * @brief Converts a 24-bit word to hexadecimal string.
 *
 * Outputs exactly 6 uppercase hexadecimal digits (e.g., "01A3BC").
 * The output buffer must be at least 7 bytes long.
 *
 * @param word 24-bit unsigned word
 * @param output Output buffer (7+ bytes)
 */
void word_to_hex(unsigned int word, char *output) {
    /* Format and store 6 hex digits, masking to 24 bits */
    sprintf(output, "%06X", word & 0xFFFFFF);
}

/**
 * @brief Converts a 24-bit word to a base64-encoded string.
 *
 * Uses a custom base64 encoding table (non-standard MIME).
 * Outputs 4 base64 characters + null terminator (5 bytes total).
 *
 * @param word 24-bit unsigned word
 * @param output Output buffer (5+ bytes)
 */
void word_to_base64(unsigned int word, char *output) {
    /* Extract and encode each 6-bit group from the word */
    output[0] = base64_table[(word >> 18) & 0x3F];  /* Bits 23-18 */
    output[1] = base64_table[(word >> 12) & 0x3F];  /* Bits 17-12 */
    output[2] = base64_table[(word >> 6) & 0x3F];   /* Bits 11-6  */
    output[3] = base64_table[word & 0x3F];          /* Bits 5-0   */
    output[4] = '\0';                               /* Null-terminate string */
} 
