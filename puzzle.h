#pragma once

#include <string>
#include <vector>

namespace Sudoku {

using PuzzleRow_t = std::vector<int>;
using PuzzleCol_t = PuzzleRow_t;
using PuzzleBoard_t = std::vector<PuzzleRow_t>;
using PuzzleCoord_t = std::pair<int, int>;

const int kBoardSquareSize = 3;
const int kBoardSize = kBoardSquareSize * kBoardSquareSize;
const int kTotalBoardSize = kBoardSize * kBoardSize;

const int kUnassigned = 0;
const char kUnassignedChar = '_';

// Class representing the state of a single Sudoku puzzle to be solved.
// We represent the board as a vector of vectors of int values, where 0
// represents no assigned value, and 1-9 represent value assignments
class Puzzle {
   public:
    // Default constructor builds an empty board
    Puzzle() : Puzzle(std::string(kTotalBoardSize, kUnassignedChar)) {}

    // Main constructor takes in the string representation of the sudoku puzzle
    Puzzle(const std::string board_string) : board_(BuildBoardVector(board_string)) {}

    PuzzleRow_t GetRow(const int row) const;
    PuzzleCol_t GetColumn(const int column) const;

    // Tests that the puzzle is of the correct dimensions, and ensures that it
    // contains only valid values (ie 1-9)
    bool IsValid() const;

    // Given a puzzle, location, and value, checks to see whether or not the value
    // is a valid assignment for the board.
    bool IsValidAssignment(const PuzzleCoord_t &coordinate, const int value) const;

    // Attempts to find an un-filled position in the board.
    // returns -1,-1 if it can't find a position, or a pair representing a
    // location otherwise.
    PuzzleCoord_t FindUnassignedPosition() const;

    // Returns the number of cells in the sudoku board
    int Size() const;

    // Generates the one-line string representation of this sudoku board
    std::string ToString() const;

    // Overloaded output operator for pretty printing the puzzle
    friend std::ostream &operator<<(std::ostream &out, const Puzzle &puzzle);

    // Overloaded input operator for loading the puzzle from a stream
    // expects the stream to contain a line containing only a sudoku puzzle
    friend std::istream &operator>>(std::istream &in, Puzzle &puzzle);

    // Convenience operator to allow accessing a row by reference instead of
    // value.
    PuzzleRow_t &operator[](const int row);

    // Helper method to build up a 2D vector representation of a sudoku board
    static PuzzleBoard_t BuildBoardVector(const std::string board_string);

   private:
    PuzzleBoard_t board_;

    bool IsLegal() const;

    bool IsValidRowAssignment(const int row, const int column, const int value) const;

    bool IsValidColumnAssignment(const int row, const int column, const int value) const;

    bool IsValidBoxAssignment(const int row, const int column, const int value) const;
};
}  // namespace Sudoku