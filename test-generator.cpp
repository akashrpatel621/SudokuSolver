#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>

#include "catch.hpp"
#include "generator.h"

using namespace Sudoku;
using Catch::Matchers::EndsWith;
using Catch::Matchers::StartsWith;

const std::string kSudokuString =
    "___8_5____3__6___7_9___38___4795_3______71_9____2__5__1____248___9____5___"
    "___6___";

TEST_CASE("GetPuzzleSourceFromUser gets correct source") {
    Generator generator;

    std::stringstream puzzle_source(kSudokuString);

    // tmpnam creates a filename that's not the name of any currently existing file
    std::string invalid_filename(std::tmpnam(nullptr));
    std::shared_ptr<std::istream> invalid_source =
        std::make_shared<std::ifstream>(invalid_filename);

    std::string valid_filename(std::tmpnam(nullptr));
    std::string valid_string = "# spf1.0\n" + kSudokuString;
    std::shared_ptr<std::istream> valid_source = std::make_shared<std::istringstream>(valid_string);

    std::ofstream valid_file(valid_filename);
    valid_file << valid_string;
    valid_file.close();

    std::shared_ptr<std::istream> incorrect_source =
        std::make_shared<std::istringstream>("not a real header");

    SECTION("ValidatePuzzleSource validates puzzle sources") {
        SECTION("Non-good streams are invalid") {
            REQUIRE_FALSE(generator.ValidatePuzzleSource(invalid_source));
        }

        SECTION("Good streams with invalid headers are invalid") {
            REQUIRE_FALSE(generator.ValidatePuzzleSource(incorrect_source));
        }

        SECTION("Good streams with valid headers are valid") {
            REQUIRE(generator.ValidatePuzzleSource(valid_source));
        }

        SECTION("Good filestreams with valid headers are valid") {
            REQUIRE(
                generator.ValidatePuzzleSource(std::make_shared<std::ifstream>(valid_filename)));
        }
    }

    SECTION("Ask for puzzle when user inputs no filename") {
        // capture cout output
        std::shared_ptr<std::ostream> cout_output = std::make_shared<std::ostringstream>();
        std::streambuf* old_buf = std::cout.rdbuf(cout_output->rdbuf());

        std::shared_ptr<std::istream> user_input =
            std::make_shared<std::istringstream>("\n" + kSudokuString);
        std::shared_ptr<std::istream> result = generator.GetPuzzleSourceFromUser(*user_input);

        // convert contents of stream to string
        std::string result_string(std::istreambuf_iterator<char>(*result), {});

        REQUIRE(result_string == kSudokuString);

        std::string output = std::dynamic_pointer_cast<std::ostringstream>(cout_output)->str();

        // reset cout output
        std::cout.rdbuf(old_buf);

        REQUIRE_THAT(output, EndsWith("Please enter in a sudoku puzzle.\n"));
    }

    SECTION("Validate file when user inputs a filename") {
        // capture cout output
        std::shared_ptr<std::ostream> cout_output = std::make_shared<std::ostringstream>();
        std::streambuf* old_buf = std::cout.rdbuf(cout_output->rdbuf());

        std::shared_ptr<std::istream> user_input =
            std::make_shared<std::istringstream>(valid_filename + "\n");
        std::shared_ptr<std::istream> result = generator.GetPuzzleSourceFromUser(*user_input);

        // convert contents of stream to string
        std::string result_string(std::istreambuf_iterator<char>(*result), {});

        REQUIRE(result_string == kSudokuString);

        std::string output = std::dynamic_pointer_cast<std::ostringstream>(cout_output)->str();

        // reset cout output
        std::cout.rdbuf(old_buf);

        REQUIRE_THAT(output, StartsWith("Please enter a filename") &&
                                 !EndsWith("Please enter in a sudoku puzzle.\n"));
    }
}