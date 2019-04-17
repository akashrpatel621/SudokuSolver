#pragma once

#include <map>
#include <vector>

#include "puzzle.h"

namespace Sudoku {

// class largely based off of
// https://www.geeksforgeeks.org/sudoku-backtracking-7/. This class focuses on
// solving sudoku puzzles.
class Solver {
 public:
  // Given a vector of puzzles to solve, returns a vector of boolean values
  // representing which puzzles were solved.
  std::vector<bool> SolvePuzzles(std::vector<Puzzle> &puzzles);

  // Attempts to solve a single puzzle, and returns true if it was able to be
  // solved.
  bool SolvePuzzle(Puzzle &puzzle);
};
}  // namespace Sudoku
