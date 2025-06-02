/**
 * @file globals.h
 * @brief Global definitions and constants for the assembler
 *
 * Centralized constants and macros for memory, syntax, encoding, and file structure.
 *
 * Author: Shimon Esterkin
 * Version: 2025A (20465)
 * ID: 207972258
 */

#ifndef GLOBALS_H
#define GLOBALS_H

/**
 * @enum AddressingMode
 * @brief Addressing types used in instruction operands
 */
typedef enum {
    ADDR_IMMEDIATE, /**< Immediate value (e.g., #5) */
    ADDR_DIRECT,    /**< Direct address (e.g., LABEL) */
    ADDR_RELATIVE,  /**< Relative address (e.g., &LABEL) */
    ADDR_REGISTER,  /**< Register (e.g., @r1) */
    ADDR_INVALID    /**< Invalid or unrecognized addressing mode */
} AddressingMode;

/* System Memory */
#define MEMORY_SIZE (1 << 21)
#define START_ADDRESS 100

/* Word Size */
#define WORD_SIZE 24
#define CONTENT_BITS 21
#define ARE_BITS 3

/* Opcode/Instruction */
#define REGISTERS_COUNT 8
#define OPCODE_COUNT 16
#define FUNCT_BITS 5
#define OPCODE_BITS 6

/* Length Limits */
#define MAX_LINE_LENGTH 81
#define MAX_FILE_NAME 256
#define MAX_DATA_VALUES 100
#define MAX_STRING_LENGTH 256
#define MAX_LABEL_LENGTH 31
#define MAX_ENTRIES 1000

/* Prefixes and Markers */
#define IMMEDIATE_PREFIX '#'
#define RELATIVE_PREFIX '&'
#define REGISTER_PREFIX '@'
#define COMMENT_CHAR ';'
#define STRING_DELIMITER '"'

/* Directive Strings */
#define DATA_DIRECTIVE ".data"
#define STRING_DIRECTIVE ".string"
#define ENTRY_DIRECTIVE ".entry"
#define EXTERN_DIRECTIVE ".extern"
#define MACRO_START "mcro"
#define MACRO_END "endmcro"

/* Value Ranges */
#define MAX_CONTENT ((1L << (CONTENT_BITS - 1)) - 1)
#define MIN_CONTENT (-(1L << (CONTENT_BITS - 1)))

/* File Extensions */
#define AS_EXTENSION ".as"
#define AM_EXTENSION ".am"
#define OB_EXTENSION ".ob"
#define ENT_EXTENSION ".ent"
#define EXT_EXTENSION ".ext"

/* ARE Bits */
#define ARE_ABSOLUTE 4
#define ARE_RELOCATABLE 2
#define ARE_EXTERNAL 1

#endif /* GLOBALS_H */
