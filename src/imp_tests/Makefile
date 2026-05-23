# ==========================================
# VARIABLES
# ==========================================

CC = gcc
CFLAGS = -Wall -Wextra -g

# Define our new output directory
BIN_DIR = bin

# The final executable now lives inside the bin directory
TARGET = $(BIN_DIR)/test_runner

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# ==========================================
# RULES
# ==========================================

# Default target
all: $(TARGET)

# 1. Link objects into the final executable
# The '| $(BIN_DIR)' means "build BIN_DIR first, but don't rebuild TARGET just because BIN_DIR's timestamp changed."
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# 2. Pattern Rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 3. Rule to create the directory
$(BIN_DIR):
	mkdir -p $@

# Clean up objects and the entire bin directory
clean:
	rm -f $(OBJS)
	rm -rf $(BIN_DIR)

.PHONY: all clean
