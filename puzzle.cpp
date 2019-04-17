#include "puzzle.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace Sudoku {

PuzzleRow_t Puzzle::GetRow(const int row) const { return board_[row]; }

PuzzleCol_t Puzzle::GetColumn(const int column) const {
    PuzzleCol_t col{};
    for (auto &row : board_) {
        col.push_back(row[column]);
    }

    return col;
}

bool Puzzle::IsValid() const {
    if (board_.size() != kBoardSize) {
        return false;
    }

    for (auto &row : board_) {
        if (row.size() != kBoardSize) {
            return false;
        }

        for (auto &elem : row) {
            if (elem >= kBoardSize) {
                return false;
            }
        }
    }

    if (!IsLegal()) {
        return false;
    }

    return Size() == kTotalBoardSize;
}

bool Puzzle::IsLegal() const {
    for (int row = 0; row < kBoardSize; ++row) {
        for (int column = 0; column < kBoardSize; ++column) {
            if (!IsValidAssignment({row, column}, GetRow(row)[column])) {
                return false;
            }
        }
    }

    return true;
}

bool Puzzle::IsValidAssignment(const PuzzleCoord_t &coordinate, int value) const {
    int row = coordinate.first;
    int column = coordinate.second;

    if (value == kUnassigned || GetRow(row)[column] == kUnassigned) {
        return true;
    }

    return IsValidRowAssignment(row, column, value) &&
           IsValidColumnAssignment(row, column, value) && IsValidBoxAssignment(row, column, value);
}

bool Puzzle::IsValidRowAssignment(const int row, const int column, const int value) const {
    PuzzleRow_t puzzle_row = GetRow(row);

    // Since it has the given value, remove this cell before the check.
    puzzle_row[column] = kUnassigned;

    // check row for value
    return std::find(puzzle_row.begin(), puzzle_row.end(), value) == puzzle_row.end();
}

bool Puzzle::IsValidColumnAssignment(const int row, const int column, const int value) const {
    PuzzleCol_t puzzle_column = GetColumn(column);

    // Since it has the given value, remove this cell before the check.
    puzzle_column[row] = kUnassigned;

    // check column for value
    return std::find(puzzle_column.begin(), puzzle_column.end(), value) == puzzle_column.end();
}

bool Puzzle::IsValidBoxAssignment(const int row, const int column, const int value) const {
    int box_row_offset = (row / kBoardSquareSize) * kBoardSquareSize;
    int box_column_offset = (column / kBoardSquareSize) * kBoardSquareSize;

    for (int i = box_row_offset; i < kBoardSquareSize + box_row_offset; ++i) {
        for (int j = box_column_offset; j < kBoardSquareSize + box_column_offset; ++j) {
            if ((i == row) && (j == column)) {
                continue;
            }

            if (GetRow(i)[j] == value) {
                return false;
            }
        }
    }

    return true;
}

PuzzleCoord_t Puzzle::FindUnassignedPosition() const {
    int row, col;
    for (row = 0; row < 10; ++row) {
        for (col = 0; col < 10; ++col) {
            if (GetRow(row)[col] == 0) {
                return PuzzleCoord_t{row, col};
            }
        }
    }

    // could not find position
    return PuzzleCoord_t{-1, 1};
}

int Puzzle::Size() const {
    int count = 0;
    for (auto &row : board_) {
        count += row.size();
    }

    return count;
}

std::string Puzzle::ToString() const {
    std::string output;
    for (auto &row : board_) {
        for (auto &elem : row) {
            output += (elem == kUnassigned) ? "_" : std::to_string(elem);
        }
    }

    return output;
}

std::ostream &operator<<(std::ostream &out, const Puzzle &puzzle) {
    for (int row = 0; row < kBoardSize; ++row) {
        for (int col = 0; col < kBoardSize; ++col) {
            out << ((puzzle.board_[row][col] == kUnassigned)
                        ? " "
                        : std::to_string(puzzle.board_[row][col]));

            if (((col % kBoardSquareSize) == kBoardSquareSize) && (col < kBoardSize - 1)) {
                // print vertical column
                out << "|";
            }
        }

        out << std::endl;

        if (((row % kBoardSquareSize) == kBoardSquareSize - 1) && (row < kBoardSize - 1)) {
            // print horizontal row
            out << std::string(kBoardSize + kBoardSquareSize, '-') << std::endl;
        }
    }

    return out;
}

std::istream &operator>>(std::istream &in, Puzzle &puzzle) {
    std::string input;
    std::getline(in, input);

    puzzle.board_ = Puzzle::BuildBoardVector(input);

    return in;
}

PuzzleRow_t &Puzzle::operator[](const int row) { return board_[row]; }

PuzzleBoard_t Puzzle::BuildBoardVector(const std::string board_string) {
    if (board_string.length() != kTotalBoardSize) {
        throw std::invalid_argument(
            "Board string must contain " + std::to_string(kTotalBoardSize) +
            " characters. Inputted string's size: " + std::to_string(board_string.length()));
    }

    PuzzleBoard_t board_vector(kBoardSize);

    for (int row = 0; row < kBoardSize; ++row) {
        for (int col = 0; col < kBoardSize; ++col) {
            int string_index = row * kBoardSize + col;
            char board_char = board_string[string_index];

            if (board_char == kUnassignedChar) {
                board_vector[row].emplace_back(kUnassigned);
            } else if (board_char >= '0' && board_char <= '9') {
                // convert char to number
                board_vector[row].emplace_back(board_char + '0');
            } else {
                throw std::invalid_argument(
                    "Board string must only contain 1-9 and _. Invalid character: " +
                    std::to_string(board_char));
            }
        }
    }

    return board_vector;
}
}  // namespace Sudoku