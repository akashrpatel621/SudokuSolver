CXX = g++
LD = g++
LDFLAGS = -g -std=c++1y
CXXFLAGS = -g -std=c++1y -Wall -Wextra -pedantic
RM = rm

TESTS = tests
TARGETS = sudoku

all : sudoku tests

clean:
	$(RM) -rf $(TARGETS) $(TESTS) *.o

sudoku: puzzle.o solver.o generator.o main.o
	$(LD) $(LDFLAGS) -o sudoku puzzle.o solver.o generator.o main.o

puzzle.o: puzzle.cpp puzzle.h
	$(CXX) -c $(CXXFLAGS) puzzle.cpp -o puzzle.o

solver.o: solver.cpp solver.h puzzle.h
	$(CXX) -c $(CXXFLAGS) solver.cpp -o solver.o

generator.o: generator.cpp generator.h puzzle.h
	$(CXX) -c $(CXXFLAGS) generator.cpp -o generator.o

main.o: main.cpp solver.h puzzle.h generator.h
	$(CXX) -c $(CXXFLAGS) main.cpp -o main.o

tests: test-main.o test-puzzle.o test-generator.o test-solver.o solver.o generator.o main.o puzzle.o
	$(LD) $(LDFLAGS) -o tests test-main.o test-puzzle.o test-generator.o test-solver.o puzzle.o solver.o generator.o

test-main.o: test-main.cpp catch.hpp
	$(CXX) -c $(CXXFLAGS) test-main.cpp -o test-main.o

test-puzzle.o: test-puzzle.cpp catch.hpp
	$(CXX) -c $(CXXFLAGS) test-puzzle.cpp -o test-puzzle.o

test-solver.o: test-solver.cpp catch.hpp
	$(CXX) -c $(CXXFLAGS) test-solver.cpp -o test-solver.o

test-generator.o: test-generator.cpp catch.hpp
	$(CXX) -c $(CXXFLAGS) test-generator.cpp -o test-generator.o

.PHONY: clean
