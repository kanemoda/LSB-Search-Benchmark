# Makefile for LSB-Search-Benchmark

# Compiler and flags
CXX       = g++
CXXFLAGS  = -std=c++17 -O2 -Wall -Iinclude
LDFLAGS   =
TARGET    = LSB-Search-Benchmark

# Directories
SRCDIR    = src
OBJDIR    = obj

# Automatically find all source files and create corresponding object files.
SOURCES   = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS   = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

.PHONY: all clean run

# Default target: build the executable.
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files.
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the executable.
run: $(TARGET)
	./$(TARGET)

# Clean up build files.
clean:
	rm -rf $(OBJDIR)/*.o $(TARGET)
