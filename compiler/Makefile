# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17

# Output binary
TARGET = jjc.exe

# Automatically gather all .cpp files in the current directory, excluding jjc.cpp
SRCS := $(filter-out jjc.cpp, $(wildcard *.cpp))

# Convert to object files
OBJS := $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files + jjc.cpp directly (without compiling main.o)
$(TARGET): $(OBJS) jjc.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^  # $^ = OBJS + jjc.cpp

# Compile each non-main .cpp file into a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
