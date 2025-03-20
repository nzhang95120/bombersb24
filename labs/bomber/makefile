CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = bomber

OBJS = main.o Map.o Point.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

main.o: main.cpp Map.h Point.h Errors.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Map.o: Map.cpp Map.h
	$(CXX) $(CXXFLAGS) -c Map.cpp

Point.o: Point.cpp Point.h
	$(CXX) $(CXXFLAGS) -c Point.cpp

clean:
	rm -f *.o $(TARGET)