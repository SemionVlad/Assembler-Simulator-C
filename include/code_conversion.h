/**
 * @file code_conversion.h
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

#ifndef CODE_CONVERSION_H
#define CODE_CONVERSION_H

#include "globals.h"
#include "cpu.h"

/**
 * @brief Converts a 24-bit word to hexadecimal string.
 *
 * Outputs exactly 6 uppercase hexadecimal digits (e.g., "01A3BC").
 * The output buffer must be at least 7 bytes long.
 *
 * @param word 24-bit unsigned word
 * @param output Output buffer (7+ bytes)
 */
void word_to_binary(unsigned int word, char *output);

/**
 * @brief Converts a 24-bit word to hexadecimal string.
 *
 * Outputs exactly 6 uppercase hexadecimal digits (e.g., "01A3BC").
 * The output buffer must be at least 7 bytes long.
 *
 * @param word 24-bit unsigned word
 * @param output Output buffer (7+ bytes)
 */
void word_to_hex(unsigned int word, char *output);

/**
 * @brief Converts a 24-bit word to a base64-encoded string.
 *
 * Uses a custom base64 encoding table (non-standard MIME).
 * Outputs 4 base64 characters + null terminator (5 bytes total).
 *
 * @param word 24-bit unsigned word
 * @param output Output buffer (5+ bytes)
 */
void word_to_base64(unsigned int word, char *output);

#endif /* CODE_CONVERSION_H */
