CXX := g++
CXXFLAGS := -m64 -Wall -O2 -fPIC -Wno-format-truncation -Wno-sign-compare -Wno-reorder -std=c++23
LDFLAGS := -g -Wall -O2
LIBS := -lm -lpthread -ldl

SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DIRS := $(sort $(dir $(OBJS)))

INCLUDE_DIRS := -I$(INCLUDE_DIR)

TARGET := main

.PHONY: all clean directories run

# Main rule
all: directories $(TARGET)

directories:
	@mkdir -p $(DIRS)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) main
