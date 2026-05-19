# Compiler and Flags
CC = clang
# The -I flag tells the compiler to look in the 'include' directory for header files
# also don't forget to link with math
CFLAGS = -g -I$(INC_DIR) -lm

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Target Executable Name
TARGET = $(BIN_DIR)/cconv

# Use wildcard to find all .c files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Use patsubst (pattern substitute) to change "src/file.c" to "build/file.o"
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Default rule to build the final executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule to compile .c files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
