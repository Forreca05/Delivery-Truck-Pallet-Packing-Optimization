# Makefile

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

# Source files
SRCS := main.cpp algorithms.cpp parser.cpp benchmark.cpp
HEADERS := algorithms.h parser.h pallet.h benchmark.h

# Output binary
TARGET := main

# Default rule
all: $(TARGET)

# Linking the final executable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

# Clean rule to remove build artifacts
clean:
	rm -f $(TARGET)

.PHONY: all clean