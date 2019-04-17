#pragma once

#include "puzzle.h"

#include <istream>
#include <memory>
#include <vector>

namespace Sudoku {

const std::string kSpfHeader = "#spf1.0";

class Generator {
   public:
    // Fetches a list of puzzles from the source specified by the user
    // This could be either via user input on commandline, or from a file.
    std::vector<Puzzle> GetPuzzlesFromUser();

    /**
     * Following methods were made public for the purposes of testing-- originally private
     */

    // Asks the user for a source of input (either file or stdin), and checks to
    // see that it's a valid source. If the user inputs a file, this fetches the
    // first line from the file to make sure that it's of the correct format as
    // provided by the kSpfHeader constant
    std::shared_ptr<std::istream> GetPuzzleSourceFromUser(std::istream& input_stream);

    // Given an istream from the user, this validates that the source is
    // valid. If the user inputs a file, this fetches the
    // first line from the file to make sure that it's of the correct format as
    // provided by the kSpfHeader constant
    bool ValidatePuzzleSource(std::shared_ptr<std::istream> puzzle_source);
};
}  // namespace Sudoku