#include "generator.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace Sudoku {

std::vector<Puzzle> Generator::GetPuzzlesFromUser() {
    std::vector<Puzzle> puzzles;
    std::string puzzle_line;

    std::shared_ptr<std::istream> puzzle_source = GetPuzzleSourceFromUser(std::cin);

    while (std::getline(*puzzle_source, puzzle_line)) {
        Puzzle p;
        try {
            std::istringstream{puzzle_line} >> p;
            puzzles.push_back(p);
        } catch (const std::exception e) {
            std::cout << "Could not read puzzle: " << puzzle_line << std::endl;
            std::cout << e.what();
        }
    }

    return puzzles;
}

std::shared_ptr<std::istream> Generator::GetPuzzleSourceFromUser(std::istream& input_stream) {
    std::string user_input = "";
    std::shared_ptr<std::istream> user_input_file;

    while (true) {
        std::cout << "Please enter a filename to read sudoku puzzles from, or just "
                     "hit enter to "
                     "provide your own puzzle."
                  << std::endl;
        std::getline(input_stream, user_input);
        user_input_file = std::make_shared<std::ifstream>(user_input.c_str());

        if (user_input == "") {
            std::cout << "Please enter in a sudoku puzzle." << std::endl;
            std::getline(input_stream, user_input);

            return std::make_shared<std::stringstream>(user_input);
        } else if (ValidatePuzzleSource(user_input_file)) {
            return user_input_file;
        }
    }
}

bool Generator::ValidatePuzzleSource(std::shared_ptr<std::istream> puzzle_source) {
    if (!puzzle_source->good()) {
        std::cout << "Provided filename does not exist! Please try again." << std::endl;

        return false;
    }

    std::string first_line;
    std::getline(*puzzle_source, first_line);

    if (first_line != "# spf1.0") {
        std::cout << "Provided filename does not contain the correct first line "
                     "('# spf1.0')."
                  << std::endl
                  << "Please try again." << std::endl;

        return false;
    }

    return true;
}

}  // namespace Sudoku