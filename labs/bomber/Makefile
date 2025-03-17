# Makefile for bomber project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Target executable name
TARGET = bomber

# Object files
OBJS = main.o Map.o Point.o

all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile main.o
main.o: main.cpp Map.h Point.h Errors.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compile Map.o
Map.o: Map.cpp Map.h
	$(CXX) $(CXXFLAGS) -c Map.cpp

# Compile Point.o
Point.o: Point.cpp Point.h
	$(CXX) $(CXXFLAGS) -c Point.cpp

# Clean build artifacts
clean:
	rm -f *.o $(TARGET)
