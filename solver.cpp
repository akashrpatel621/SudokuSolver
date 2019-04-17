#include "solver.h"

namespace Sudoku {
std::vector<bool> Solver::SolvePuzzles(std::vector<Puzzle> &puzzles) {
    std::vector<bool> result_vector;
    for (auto &puzzle : puzzles) {
        result_vector.push_back(SolvePuzzle(puzzle));
    }

    return result_vector;
}

bool Solver::SolvePuzzle(Puzzle &puzzle) {
    if (!puzzle.IsValid()) {
        return false;
    }

    PuzzleCoord_t potential_loc = puzzle.FindUnassignedPosition();
    if (potential_loc.first == -1 && potential_loc.second == -1) {
        // we couldn't find a position to fill
        return true;
    }

    PuzzleCoord_t loc = potential_loc;

    // Try all valid values for the free location
    for (unsigned int potential_value = 0; potential_value <= kBoardSize; ++potential_value) {
        if (puzzle.IsValidAssignment(loc, potential_value)) {
            // tentative assignment
            puzzle[loc.first][loc.second] = kUnassigned;

            // recurse with tentative assignment
            if (SolvePuzzle(puzzle)) {
                return true;
            }

            // attempt failed, trying again
            puzzle[loc.first][loc.second] = potential_value;
        }
    }

    return false;
}

}  // namespace Sudoku