CXX := g++
CXXFLAGS := -m64 -Wall -O2 -fPIC -Wno-format-truncation -Wno-sign-compare -std=c++14
LDFLAGS := -g -Wall -O2
LIBS := -lcplex -lm -lpthread -ldl

CPLEX_DIR := /opt/ibm/ILOG/CPLEX_Studio2211/cplex
CPLEX_INCLUDE_DIR := $(CPLEX_DIR)/include
CPLEX_LIB_DIR := $(CPLEX_DIR)/lib/x86-64_linux/static_pic

SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := bin

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DIRS := $(sort $(dir $(OBJS)) $(BIN_DIR))

INCLUDE_DIRS := -I$(CPLEX_INCLUDE_DIR) -I$(INCLUDE_DIR)
LIB_DIRS := -L$(CPLEX_LIB_DIR)

TARGET := $(BIN_DIR)/main

.PHONY: all clean directories run

# Main rule
all: directories $(TARGET)

directories:
	@mkdir -p $(DIRS)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(LIB_DIRS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)