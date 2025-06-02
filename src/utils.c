/**
 * @file utils.c
 * @brief General Utility Function Implementation
 *
 * Implements memory-safe allocation, string manipulation,
 * file checking, filename formatting, and output displays.
 *
 * Author: Shimon Esterkin
 * ID: 207972258
 * Version: 2025A (20465)
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "utils.h"
#include "errors.h"

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
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        report_error(ERROR_MEMORY, "Memory allocation failed (%zu bytes)", size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/**
 * @brief Duplicate a string safely with dynamic memory.
 *
 * @param str String to duplicate
 * @return char* Pointer to duplicated string
 */
char *safe_strdup(const char *str) {
    char *copy = safe_malloc(strlen(str) + 1);
    strcpy(copy, str);
    return copy;
}

/* -------------------------
   File Handling
   ------------------------- */
   
/**
 * @brief Check if a file exists on disk.
 *
 * @param filename File path to check
 * @return int 1 if exists, 0 otherwise
 */
int file_exists(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp) {
        fclose(fp);
        return 1;
    }
    return 0;
}
   
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
char *create_filename(const char *original, const char *new_ext) {
    const char *dot = strrchr(original, '.');
    size_t base_len = dot ? (size_t)(dot - original) : strlen(original);
    char *new_name = safe_malloc(base_len + strlen(new_ext) + 1);
    strncpy(new_name, original, base_len);
    new_name[base_len] = '\0';
    strcat(new_name, new_ext);
    return new_name;
}

/**
 * @brief Build full path to output file in appropriate directory
 *
 * @param original_filename Name of the input source file
 * @param subdir Subdirectory to store output (e.g. "ob", "ent", "ext")
 * @param new_ext New file extension (e.g. ".ob")
 * @return char* Dynamically allocated full output path
 */
char *create_output_path(const char *original_filename, const char *subdir, const char *new_ext) {
    const char *slash = strrchr(original_filename, '/');
    const char *backslash = strrchr(original_filename, '\\');
    const char *base_name = slash ? slash + 1 : (backslash ? backslash + 1 : original_filename);
    const char *dot = strrchr(base_name, '.');
    size_t base_len = dot ? (size_t)(dot - base_name) : strlen(base_name);

    size_t path_len = strlen("Tests/output_files/") + strlen(subdir) + 1 + base_len + strlen(new_ext) + 1;
    char *full_path = safe_malloc(path_len);

    sprintf(full_path, "Tests/output_files/%s/%.*s%s", subdir, (int)base_len, base_name, new_ext);
    return full_path;
}

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
void normalize_string(char *str, int collapse_spaces) {
    char *src = str;
    char *dst = str;
    int in_space = 0;

    while (isspace((unsigned char)*src)) {
        src++;
    }

    while (*src) {
        if (isspace((unsigned char)*src)) {
            if (!in_space) {
                *dst++ = ' ';
                in_space = collapse_spaces;
            }
        } else {
            *dst++ = *src;
            in_space = 0;
        }
        src++;
    }

    if (dst > str && isspace((unsigned char)*(dst - 1))) {
        dst--;
    }

    *dst = '\0';
}

/* -------------------------
   Console I/O
   ------------------------- */

/**
 * @brief Print welcome banner for the program.
 */
void display_welcome(void) {
    printf(
        "===============================================================================\n"
        "               Maman14 - Assembly Simulation Project (C)                      \n"
        "===============================================================================\n"
        "  Author : Shimon Esterkin\n"
        "  ID     : 207972258\n"
        "  Course : System Programming Laboratory (20465)\n"
        "  Version: 2025A\n"
        "===============================================================================\n\n"
    );
}

/**
 * @brief Print help message to stdout.
 */
void display_help(void) {
    printf(
        "Usage:\n"
        "  assembler [options] file1.as [file2.as ...]\n\n"
        "Options:\n"
        "  -h, --help      Display help information\n"
        "  -v, --version   Show version and author info\n\n"
        "Expected Input:\n"
        "  Files with .as extension (assembly source files)\n"
        "Generated Output:\n"
        "  .am  - After macro expansion\n"
        "  .ob  - Encoded object\n"
        "  .ent - Entry symbols\n"
        "  .ext - External symbols\n"
    );
}

/**
 * @brief Print version information to stdout.
 */
void display_version(void) {
    printf(
        "Maman14 - Two-Pass Assembler (20465)\n"
        "Author : Shimon Esterkin\n"
        "ID     : 207972258\n"
        "Course : System Programming Laboratory (20465)\n"
        "Build  : 2025A\n"
    );
}
