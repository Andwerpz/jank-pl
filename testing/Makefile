# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Iinclude

# Entry point
ENTRY := ./main.cpp

# Output binary name
TARGET := ./main.exe

# Find all .cpp files (recursively), exclude jjc.cpp
SRCS := $(filter-out $(ENTRY), $(shell find . -name '*.cpp'))
OBJS := $(SRCS:%.cpp=build/%.o)

# Ensure output directories exist
DIRS := $(sort $(dir $(OBJS)))

# Default target
all: $(TARGET)

# Build the final binary
$(TARGET): $(OBJS) build/$(subst .cpp,.o,$(ENTRY))
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile .cpp to .o into build/ directory
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf build $(TARGET)

.PHONY: all clean
