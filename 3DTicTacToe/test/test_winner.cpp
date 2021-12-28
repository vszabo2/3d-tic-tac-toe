#include "catch.hpp"
#include "get_winner.h"

using namespace vszabo2ttt;

TEST_CASE("3x3x3") {
    Board board(3);

    SECTION("One Move") {
        Position p = {0, 1, 2};
        board[p] = 1;
        REQUIRE_FALSE(IsWinningMove(board, p));
    }

    SECTION("X win") {
        board[{0, 1, 1}] = 2;
        board[{1, 1, 1}] = 2;
        board[{2, 1, 1}] = 2;
        REQUIRE(IsWinningMove(board, {0, 1, 1}));
        REQUIRE(IsWinningMove(board, {1, 1, 1}));
        REQUIRE(IsWinningMove(board, {2, 1, 1}));
    }

    SECTION("Y win") {
        board[{2, 0, 1}] = 3;
        board[{2, 1, 1}] = 3;
        board[{2, 2, 1}] = 3;
        REQUIRE(IsWinningMove(board, {2, 0, 1}));
        REQUIRE(IsWinningMove(board, {2, 1, 1}));
        REQUIRE(IsWinningMove(board, {2, 2, 1}));
    }

    SECTION("No Z win") {
        board[{0, 0, 0}] = 0;
        board[{0, 0, 1}] = 1;
        board[{0, 0, 2}] = 2;
        REQUIRE_FALSE(IsWinningMove(board, {0, 0, 0}));
        REQUIRE_FALSE(IsWinningMove(board, {0, 0, 1}));
        REQUIRE_FALSE(IsWinningMove(board, {0, 0, 2}));
    }

    SECTION("X_DIAG1") {
        board[{0, 0, 0}] = 2;
        board[{0, 1, 1}] = 2;
        board[{0, 2, 2}] = 2;
        REQUIRE(IsWinningMove(board, {0, 0, 0}));
        REQUIRE(IsWinningMove(board, {0, 1, 1}));
        REQUIRE(IsWinningMove(board, {0, 2, 2}));
    }

    SECTION("X_DIAG2") {
        board[{1, 0, 2}] = 1;
        board[{1, 1, 1}] = 1;
        board[{1, 2, 0}] = 1;
        REQUIRE(IsWinningMove(board, {1, 0, 2}));
        REQUIRE(IsWinningMove(board, {1, 1, 1}));
        REQUIRE(IsWinningMove(board, {1, 2, 0}));
    }

    SECTION("CUBE_DIAG1") {
        board[{0, 0, 0}] = 3;
        board[{1, 1, 1}] = 3;
        board[{2, 2, 2}] = 3;
        REQUIRE(IsWinningMove(board, {0, 0, 0}));
        REQUIRE(IsWinningMove(board, {1, 1, 1}));
        REQUIRE(IsWinningMove(board, {2, 2, 2}));
    }

    SECTION("CUBE_DIAG2") {
        board[{0, 0, 2}] = 4;
        board[{1, 1, 1}] = 4;
        board[{2, 2, 0}] = 4;
        REQUIRE(IsWinningMove(board, {0, 0, 2}));
        REQUIRE(IsWinningMove(board, {1, 1, 1}));
        REQUIRE(IsWinningMove(board, {2, 2, 0}));
    }

    SECTION("CUBE_DIAG3") {
        board[{0, 2, 0}] = 5;
        board[{1, 1, 1}] = 5;
        board[{2, 0, 2}] = 5;
        REQUIRE(IsWinningMove(board, {0, 2, 0}));
        REQUIRE(IsWinningMove(board, {1, 1, 1}));
        REQUIRE(IsWinningMove(board, {2, 0, 2}));
    }

    SECTION("CUBE_DIAG4") {
        board[{0, 2, 2}] = 6;
        board[{1, 1, 1}] = 6;
        board[{2, 0, 0}] = 6;
        REQUIRE(IsWinningMove(board, {0, 2, 2}));
        REQUIRE(IsWinningMove(board, {1, 1, 1}));
        REQUIRE(IsWinningMove(board, {2, 0, 0}));
    }
}
