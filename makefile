# -------------------------------------------------------------
# Maman14 - Assembler Simulation Project
# Author  : Shimon Esterkin
# ID      : 207972258
# Course  : System Programming Laboratory (20465)
# Version : 2025A
# -------------------------------------------------------------

# ------------------- Compiler Settings -------------------
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -Iinclude

# ------------------- Directory Structure -------------------
SRC_DIR = src
BUILD_DIR = build
TEST_INPUTS_DIR = Tests/input_files/as
TEST_MODULES_DIR = Tests/project_files_tests
OUTPUT_DIRS = Tests/output_files/am Tests/output_files/ob Tests/output_files/ent Tests/output_files/ext

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
EXEC = assembler

# ------------------- Targets -------------------
all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(EXEC)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------- Run Tests -------------------
test: all
	@echo "📦 Running assembler tests from $(TEST_INPUTS_DIR):"
	@for file in $(wildcard $(TEST_INPUTS_DIR)/*.as); do \
		echo ">>> 🔧 Assembling $$file"; \
		./$(EXEC) "$$file"; \
	done

# ------------------- Project Module Tests -------------------
test_preproc: all
	@$(CC) $(CFLAGS) $(TEST_MODULES_DIR)/test_preproc.c -o $(TEST_MODULES_DIR)/test_preproc
	@for file in $(wildcard $(TEST_INPUTS_DIR)/*.as); do \
		echo "🔍 Preprocessing $$file"; \
		$(TEST_MODULES_DIR)/test_preproc "$$file"; \
	done

test_first_pass: all
	@$(CC) $(CFLAGS) $(TEST_MODULES_DIR)/test_first_pass.c -o $(TEST_MODULES_DIR)/test_first_pass
	@for file in $(wildcard $(TEST_INPUTS_DIR)/*.as); do \
		echo "🔍 Running First Pass on $$file"; \
		$(TEST_MODULES_DIR)/test_first_pass "$$file"; \
	done

test_second_pass: all
	@$(CC) $(CFLAGS) $(TEST_MODULES_DIR)/test_second_pass.c -o $(TEST_MODULES_DIR)/test_second_pass
	@for file in $(wildcard $(TEST_INPUTS_DIR)/*.as); do \
		echo "🔍 Running Second Pass on $$file"; \
		$(TEST_MODULES_DIR)/test_second_pass "$$file"; \
	done

# ------------------- Clean -------------------
clean:
	@echo "🧹 Deleting object files, build files, executable, and output files:"
	@rm -fv $(BUILD_DIR)/*.o $(EXEC)
	@rm -fv $(TEST_INPUTS_DIR)/*.am $(TEST_INPUTS_DIR)/*.ob $(TEST_INPUTS_DIR)/*.ent $(TEST_INPUTS_DIR)/*.ext
	@for dir in $(OUTPUT_DIRS); do \
		rm -fv $$dir/*; \
	done
	@rm -fv Tests/output_files/experiment_logs/*.log 2>/dev/null || true

rebuild: clean all

.PHONY: all clean rebuild test test_preproc test_first_pass test_second_pass
