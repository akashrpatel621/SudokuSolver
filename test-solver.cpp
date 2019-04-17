#include "catch.hpp"
#include "solver.h"

const std::string kSudokuString =
    "___8_5____3__6___7_9___38___4795_3______71_9____2__5__1____248___9____5___"
    "___6___";

const std::string kInvalidSudokuString =
    "___8_5____3__6___7_9___38___4795_3______71_9____2__5__1____248___9____5______66__";

using namespace Sudoku;
TEST_CASE("Solver can solve puzzles") {
    Solver s;

    SECTION("Valid puzzle") {
        Puzzle p(kSudokuString);
        REQUIRE(s.SolvePuzzle(p));
    }

    SECTION("Invalid Puzzle") {
        Puzzle p(kInvalidSudokuString);
        REQUIRE_FALSE(s.SolvePuzzle(p));
    }
}