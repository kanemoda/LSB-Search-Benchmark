# Makefile for LSB-Search-Benchmark

# Compiler and flags
CXX       = g++
CXXFLAGS  = -std=c++17 -O2 -Wall -Iinclude
LDFLAGS   =

# Directories
SRCDIR    = src
OBJDIR    = obj

# Targets
TARGET_SINGLE = LSB-Search-Benchmark-Single
TARGET_MULTI  = LSB-Search-Benchmark-Multi

# Source files
SOURCES_COMMON = $(SRCDIR)/debruijn.cpp $(SRCDIR)/iterative.cpp $(SRCDIR)/intrinsics.cpp
SOURCES_SINGLE = $(SRCDIR)/main_single.cpp
SOURCES_MULTI  = $(SRCDIR)/main_multi.cpp

# Object files
OBJECTS_COMMON = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES_COMMON))
OBJECTS_SINGLE = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES_SINGLE))
OBJECTS_MULTI  = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES_MULTI))

.PHONY: all clean run-single run-multi

all: $(TARGET_SINGLE) $(TARGET_MULTI)

$(TARGET_SINGLE): $(OBJECTS_COMMON) $(OBJECTS_SINGLE)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET_MULTI): $(OBJECTS_COMMON) $(OBJECTS_MULTI)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run-single: $(TARGET_SINGLE)
	./$(TARGET_SINGLE)

run-multi: $(TARGET_MULTI)
	./$(TARGET_MULTI)

clean:
	rm -rf $(OBJDIR)/*.o $(TARGET_SINGLE) $(TARGET_MULTI)
