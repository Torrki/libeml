# Compiler and flags
CC = gcc
CFLAGS = -Wall
LDFLAGS = -lpthread

# Directories
SRC_DIR = .
BUILD_DIR = ./build

# Default source file and executable
SRC ?= $(SRC_DIR)/equeue.c $(SRC_DIR)/mlcontext.c $(SRC_DIR)/mainloop.c
EXEC ?= $(BUILD_DIR)/mainloop

# Object files (located in the build directory)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Default target: build the executable
all: $(EXEC)

# Rule to create the executable from the object file
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Rule to compile the C source file into an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean rule to remove generated files
clean:
	rm -rf $(BUILD_DIR)

# Rebuild everything from scratch
rebuild: clean all

