CXX = g++
CXXFLAGS = -O3 -march=native -fopenmp -std=c++17

TARGET = program

all: $(TARGET)

$(TARGET): main.o correlate.o
	$(CXX) $(CXXFLAGS) main.o correlate.o -o $(TARGET)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

correlate.o: correlate.cpp
	$(CXX) $(CXXFLAGS) -c correlate.cpp

clean:
	rm -f *.o $(TARGET)
