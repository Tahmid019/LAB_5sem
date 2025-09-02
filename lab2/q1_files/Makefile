# Compiler
CC = gcc

# Compiler flags
# -g: adds debugging information
# -Wall: enables all compiler's warning messages
CFLAGS = -g -Wall

# The target executable
TARGET = process_tree

# The source files
SOURCES = main.c graph.c

# The object files
OBJECTS = $(SOURCES:.c=.o)

# Default rule
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile a .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the directory
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean