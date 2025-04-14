# Compiler and flags
CXX := g++
CXXFLAGS := -m64 -Wall -O2 -fPIC -Wno-format-truncation -Wno-sign-compare -std=c++14
LDFLAGS := -g -Wall -O2
LIBS := -lcplex -lm -lpthread -ldl

# Paths
CPLEX_DIR := /opt/ibm/ILOG/CPLEX_Studio2211/cplex
CPLEX_INCLUDE_DIR := $(CPLEX_DIR)/include
CPLEX_LIB_DIR := $(CPLEX_DIR)/lib/x86-64_linux/static_pic

# Project directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := bin

# Find all source files recursively
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
# Generate object file paths in build directory
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
# Create list of directories to be created
DIRS := $(sort $(dir $(OBJS)) $(BIN_DIR))

# Include paths
INCLUDE_DIRS := -I$(CPLEX_INCLUDE_DIR) -I$(INCLUDE_DIR)
LIB_DIRS := -L$(CPLEX_LIB_DIR)

# Target executable
TARGET := $(BIN_DIR)/main

# Phony targets
.PHONY: all clean directories run

# Main rule
all: directories $(TARGET)

# Create directory structure
directories:
	@mkdir -p $(DIRS)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(LIB_DIRS) -o $@ $^ $(LIBS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run target
run: all
	./$(TARGET) $(ARGS)