# Compiler
CC = gcc

# Preprocessor flags (for -I, -D, etc.)
# These tell the compiler where to find your header files.
CPPFLAGS = -I./init_data \
           -I./features \
           -I./common

# Compilation flags
# -g: Include debugging information
# -O0: No optimization (better for debugging)
# -std=c99: Use the C99 standard
# -Wall: Enable all compiler warnings
CFLAGS = -g -O0 -std=c99 -Wall

# Linker flags (if you need to link specific libraries, e.g., -lm for math)
LDFLAGS =

# Target executable name
TARGET = library_system

# Source files
# List all your .c files, including their paths from the root directory.
SRCS = main.c \
       init_data/initialization.c \
       init_data/users.c \
       init_data/book_tree.c \
       init_data/genre_hash.c \
       init_data/author_hash.c \
       features/loan_management.c \
       features/bestseller.c \
       features/book_management.c \
       features/user_management.c \
       common/date_utils.c \
       common/queue.c \
       common/list.c \
       common/book_stack.c

# Object files: Automatically generate .o filenames from .c filenames
# e.g., main.c becomes main.o, init_data/users.c becomes init_data/users.o
OBJS = $(SRCS:.c=.o)

# Default target: This is what 'make' will do if you don't specify a target.
# It depends on the $(TARGET) executable.
all: $(TARGET)

# Rule to link all object files into the final executable
# $(CC) is gcc
# $(CFLAGS) includes -g for debugging symbols in the final executable
# -o $(TARGET) specifies the output file name
# $(OBJS) lists all object files to link
# $(LDFLAGS) includes any specific linker flags
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Pattern rule to compile .c source files into .o object files
# For any file ending in .o, this rule looks for a corresponding .c file.
# $< is an automatic variable representing the first prerequisite (the .c file).
# $@ is an automatic variable representing the target file (the .o file).
# This rule correctly handles source files in subdirectories because $(OBJS)
# will contain paths like 'init_data/users.o', and make will find 'init_data/users.c'.
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Clean rule: Remove the executable and all object files
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets: These are targets that are not actual files.
# 'all' and 'clean' are common phony targets.
.PHONY: all clean
