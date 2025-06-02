/**
 * @file utils.h
 * @brief General Utility Interface
 *
 * Utility helper functions for memory allocation, string and file operations.
 * Shared across all assembler modules.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

/* -------------------------
   Memory Allocation
   ------------------------- */
   
/**
 * @brief Safely allocate memory with error handling.
 *
 * Terminates the program if allocation fails.
 *
 * @param size Number of bytes to allocate
 * @return void* Pointer to allocated memory block
 * 
 * @note Exits the program immediately if allocation fails.
 */
void *safe_malloc(size_t size);

/**
 * @brief Duplicate a string safely with dynamic memory.
 *
 * @param str String to duplicate
 * @return char* Pointer to duplicated string
 */
char *safe_strdup(const char *str);

/* -------------------------
   File Handling
   ------------------------- */
   
/**
 * @brief Check if a file exists on disk.
 *
 * @param filename File path to check
 * @return int 1 if exists, 0 otherwise
 */
int file_exists(const char *filename);
   
/**
 * @brief Generate a new filename by replacing its extension.
 *
 * If the original filename contains a period ('.'), it is replaced.
 * Otherwise, the extension is appended.
 *
 * @param original Original filename
 * @param new_ext New extension (e.g. ".am")
 * @return char* Allocated string containing new filename
 */
char *create_filename(const char *original, const char *new_ext);

/**
 * @brief Build full path to output file in appropriate directory
 *
 * @param original_filename Name of the input source file
 * @param subdir Subdirectory to store output (e.g. "ob", "ent", "ext")
 * @param new_ext New file extension (e.g. ".ob")
 * @return char* Dynamically allocated full output path
 */
char *create_output_path(const char *original_filename, const char *subdir, const char *new_ext);

/* -------------------------
   String Utilities
   ------------------------- */
   
/**
 * @brief Normalize string by removing redundant whitespace.
 *
 * Modifies the string in-place:
 * - Trims leading and trailing spaces
 * - Collapses multiple spaces to one
 * - Optionally converts tabs to spaces
 *
 * @param str The input string to normalize
 * @param collapse_spaces If true, collapse multiple spaces into one
 */
void normalize_string(char *str, int collapse_spaces);

/* -------------------------
   Console I/O
   ------------------------- */

/**
 * @brief Print welcome banner for the program.
 */
void display_welcome(void);

/**
 * @brief Print help message to stdout.
 */
void display_help(void);

/**
 * @brief Print version information to stdout.
 */
void display_version(void);

#endif /* UTILS_H */ 
