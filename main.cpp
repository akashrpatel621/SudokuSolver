#include "generator.h"
#include "solver.h"

#include <iostream>
#include <sstream>

int main() {
  Sudoku::Generator generator;
  Sudoku::Solver solver;

  std::vector<Sudoku::Puzzle> puzzles = generator.GetPuzzlesFromUser();

  for (auto& puzzle : puzzles) {
    std::cout << "Solving puzzle : " << std::endl << puzzle << std::endl;
    if (solver.SolvePuzzle(puzzle)) {
      std::cout << "Successfully solved puzzle!" << std::endl;
      std::cout << puzzle << std::endl;
    } else {
      std::cout << "Unable to solve puzzle!" << std::endl;
    }
  }

  return 0;
}