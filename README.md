# Maman 14 – Two-Pass Assembler in C

## Author
- **Name**: Shimon Esterkin  
- **ID**: *****2258  
- **Course**: System Programming Laboratory (20465)  
- **Semester**: 2025A

## Project Overview

This project is a complete simulation of a two-pass assembler written in C90. It processes assembly source files with `.as` extension, performs macro expansion, builds a symbol table, validates input, and produces output files in a specific format.

### Features Implemented:
- Macro expansion (`.am` generation)
- Symbol table management with support for `.entry` and `.extern`
- First pass: label collection, `.data` and `.string` storage
- Second pass: final instruction encoding and output generation
- File outputs: `.ob`, `.ent`, `.ext`

## Folder Structure

```
AssemblerSimulationMaman14ShimonEsterkin/
├── assembler                   # Executable output
├── include/                    # Header files
├── src/                        # Source files
├── build/                      # Object files (.o)
├── Tests/
│   ├── input_files/as/         # 10 test input files: valid1–5.as, invalid1–5.as
│   ├── output_files/
│   │   ├── am/                 # Macro-expanded .am files
│   │   ├── ob/                 # Object files generated
│   │   ├── ent/                # Entry symbol files
│   │   ├── ext/                # External symbol files
│   │   ├── test_run_log.txt/   #Saved test outputs from terminal
│   └── project_files_tests/    # Manual test runners for each pass
├── README.md                   # This file
├── makefile                    # Makefile for compilation and testing
```

## Compilation

```bash
make        # Compile all source files
make clean  # Remove all object, build, and output files
```

## Usage

```bash
./assembler <file.as> [<file2.as> ...]
```

## Automated Testing

```bash
make test
```

Runs all `.as` files inside `Tests/input_files/as/` and stores all output files under `Tests/output_files/`.

### Note on Test Inputs
The project includes:
- **5 valid** test cases: `valid1.as` to `valid5.as`
- **5 invalid** test cases: `invalid1.as` to `invalid5.as`

Each invalid file was crafted to trigger a specific error:
| Filename       | Error Description |
|----------------|-------------------|
| invalid1.as    | General parsing error |
| invalid2.as    | Invalid directive usage |
| invalid3.as    | Duplicate label definition: `M1` |
| invalid4.as    | Multiple symbol redefinitions: `START`, `EXT1` |
| invalid5.as    | General syntax or validation error |

## Compliance & Standards

- Fully **ISO C90 compliant**
- Compiled using: `gcc -Wall -ansi -pedantic`
- No external libraries used

## 📄 License
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

---
Created by [SemionVlad](https://github.com/SemionVlad) for academic and portfolio purposes. This repository is intended for **non-commercial**, **educational** sharing only.
