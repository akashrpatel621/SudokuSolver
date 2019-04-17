#include "catch.hpp"
#include "puzzle.h"

#include <sstream>

using namespace Sudoku;
using Catch::Matchers::Equals;
using Catch::Matchers::StartsWith;

const std::string kSudokuString =
    "___8_5____3__6___7_9___38___4795_3______71_9____2__5__1____248___9____5___"
    "___6___";

const PuzzleBoard_t kTestBoard = {
    {0, 0, 0, 8, 0, 5, 0, 0, 0}, {0, 3, 0, 0, 6, 0, 0, 0, 7}, {0, 9, 0, 0, 0, 3, 8, 0, 0},
    {0, 4, 7, 9, 5, 0, 3, 0, 0}, {0, 0, 0, 0, 7, 1, 0, 9, 0}, {0, 0, 0, 2, 0, 0, 5, 0, 0},
    {1, 0, 0, 0, 0, 2, 4, 8, 0}, {0, 0, 9, 0, 0, 0, 0, 5, 0}, {0, 0, 0, 0, 0, 6, 0, 0, 0}};

TEST_CASE("Puzzles can be instantiated and accessed", "[puzzle]") {
    Puzzle puzzle;

    SECTION("Default puzzle contains all spaces") {
        REQUIRE(puzzle.Size() == kTotalBoardSize);
        REQUIRE_THAT(puzzle.ToString(), Equals(std::string(kTotalBoardSize, kUnassignedChar)));
    }

    SECTION("[] operator can be used to modify the puzzle") {
        SECTION("[] can assign a new vector for a row") {
            puzzle[0] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

            REQUIRE(puzzle.Size() == kTotalBoardSize);
            REQUIRE_THAT(puzzle[0], Equals(PuzzleRow_t{1, 2, 3, 4, 5, 6, 7, 8, 9}));
        }

        SECTION("[] can add new value to mutable reference") {
            PuzzleRow_t &test_row = puzzle[0];
            test_row.push_back(0);

            REQUIRE(puzzle.Size() == kTotalBoardSize + 1);
            REQUIRE_THAT(puzzle[0], Equals(PuzzleRow_t(kBoardSize + 1, 0)));
        }

        SECTION("[] can directly expand a row") {
            puzzle[0].push_back(0);

            REQUIRE(puzzle.Size() == kTotalBoardSize + 1);
            REQUIRE_THAT(puzzle[0], Equals(PuzzleRow_t(kBoardSize + 1, 0)));
        }
    }

    SECTION("Column accessor returns columns correctly") {
        for (unsigned int i = 0; i < kBoardSize; ++i) {
            puzzle[i][4] = i;
        }

        REQUIRE_THAT(puzzle.GetColumn(4), Equals(PuzzleRow_t{0, 1, 2, 3, 4, 5, 6, 7, 8}));
    }

    SECTION("Row accessor returns rows correctly") {
        puzzle[0] = PuzzleRow_t(kBoardSize, 5);
        REQUIRE_THAT(puzzle.GetRow(0), Equals(PuzzleRow_t(kBoardSize, 5)));
    }

    SECTION("Puzzle can be instatiated with a board") {
        Puzzle filled_puzzle(kSudokuString);
        REQUIRE_THAT(filled_puzzle[0], Equals(PuzzleRow_t{0, 0, 0, 8, 0, 5, 0, 0, 0}));
    }
}

TEST_CASE("Puzzles can do rudimentary validity checking", "[puzzle]") {
    Puzzle puzzle;
    REQUIRE(puzzle.IsValid());

    SECTION("Puzzles aren't valid if they contain too many cells") {
        puzzle[0].push_back(0);
        REQUIRE(!puzzle.IsValid());
    }

    SECTION("Puzzles aren't valid if it contains too few cells") {
        puzzle[0].pop_back();
        REQUIRE(!puzzle.IsValid());
    }

    SECTION("Puzzles aren't valid if rows aren't correct size") {
        puzzle[0].pop_back();
        puzzle[1].push_back(0);
        REQUIRE(!puzzle.IsValid());
    }

    SECTION("Puzzles aren't valid if they don't contain the correct characters") {
        puzzle[0] = {1, 2, 3, 4, 5, 6, 7, 8, 10};
        REQUIRE(!puzzle.IsValid());
    }

    SECTION("Puzzles are valid if they contain the correct characters") {
        puzzle[0] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        REQUIRE(puzzle.IsValid());
    }
}

TEST_CASE("Puzzles can be serialized and deserialized", "[puzzle]") {
    Puzzle puzzle;
    REQUIRE(puzzle.IsValid());
    REQUIRE(puzzle.Size() == kTotalBoardSize);
    REQUIRE_THAT(puzzle[0], Equals(PuzzleRow_t(kBoardSize, 0)));

    std::stringstream puzzle_stream(kSudokuString);
    puzzle_stream >> puzzle;

    SECTION("Puzzle can have a new board streamed in") {
        REQUIRE(puzzle.IsValid());
        REQUIRE(puzzle.Size() == kTotalBoardSize);
        REQUIRE_THAT(puzzle[0], Equals(PuzzleRow_t{0, 0, 0, 8, 0, 5, 0, 0, 0}));
    }

    SECTION("Puzzle can serialize back to original string") {
        REQUIRE_THAT(puzzle.ToString(), Equals(kSudokuString));
    }

    SECTION("Puzzle can be pretty printed") {
        std::stringstream output_stream;
        output_stream << puzzle;

        std::string board_line;

        // first line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals("   |8 5|   "));

        // second line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals(" 3 | 6 |  7"));

        // third line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals(" 9 |  3|8  "));

        // fourth line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals(std::string(11, '-')));

        // fifth line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals(" 47|95 |3  "));

        // sixth line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals("   | 71| 9 "));

        // seventh line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals("   |2  |5  "));

        // eigth line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals(std::string(11, '-')));

        // ninth line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals("1  |  2|48 "));

        // tenth line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals("  9|   | 5 "));

        // eleventh line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals("   |  6|   "));

        // twelwth line
        std::getline(output_stream, board_line);
        CHECK_THAT(board_line, Equals(std::string(11, '-')));
    }
}

TEST_CASE("Puzzle class can build puzzle boards via BuildBoardVector", "[puzzle]") {
    SECTION("Puzzle boards can be built from valid puzzle strings") {
        PuzzleBoard_t built_board = Puzzle::BuildBoardVector(kSudokuString);
        REQUIRE_THAT(built_board, Equals(kTestBoard));
    }

    SECTION("Puzzle boards can't be built with invalid characters") {
        PuzzleBoard_t built_board = Puzzle::BuildBoardVector(std::string(81, '0'));
        REQUIRE_THAT(built_board, Equals(PuzzleBoard_t(kBoardSize, PuzzleRow_t(kBoardSize, '0'))));
    }

    SECTION("Puzzle boards can't be built from too-short strings") {
        REQUIRE_THROWS_WITH(Puzzle::BuildBoardVector("__"),
                            StartsWith("Board string must contain"));
    }

    SECTION("Puzzle boards can't be built from strings with invalid characters") {
        SECTION("Letter characters") {
            REQUIRE_THROWS_WITH(Puzzle::BuildBoardVector(std::string(81, 'a')),
                                StartsWith("Board string must only contain 1-9 and _."));
        }

        SECTION("Invalid number characters") {
            REQUIRE_THROWS_WITH(Puzzle::BuildBoardVector(std::string(81, '0')),
                                StartsWith("Board string must only contain 1-9 and _."));
        }
    }
}