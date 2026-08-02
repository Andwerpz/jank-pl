# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Iinclude -g -O2
LDLIBS := -lcrypto

# Entry point
ENTRY := ./src/dylan.cpp

# Output binary name
TARGET := ./dylan.exe

# Find all .cpp files (recursively), exclude jjc.cpp
SRCS := $(filter-out $(ENTRY), $(shell find . -name '*.cpp'))
OBJS := $(SRCS:%.cpp=build/%.o)

# Ensure output directories exist
DIRS := $(sort $(dir $(OBJS)))

# Default target
all: $(TARGET)

# Build the final binary
$(TARGET): $(OBJS) build/$(subst .cpp,.o,$(ENTRY))
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

# Compile .cpp to .o into build/ directory
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf build $(TARGET)

# Symlink to /usr/local/bin/dylan
install: $(TARGET)
	sudo ln -sf $(abspath $(TARGET)) /usr/local/bin/dylan

# Remove symlink
uninstall:
	sudo rm -f /usr/local/bin/dylan

.PHONY: all clean
