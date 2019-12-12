#include "get_winner.h"

#include <bitset>

namespace cs126ttt {

enum : unsigned char {
    X_WIN = 0,
    Y_WIN,
    Z_WIN,
    X_DIAG1,
    X_DIAG2,
    Y_DIAG1,
    Y_DIAG2,
    Z_DIAG1,
    Z_DIAG2,
    CUBE_DIAG1,
    CUBE_DIAG2,
    CUBE_DIAG3,
    CUBE_DIAG4,
    FIELD_SIZE
};

std::bitset<FIELD_SIZE>::reference operator&=(
    std::bitset<FIELD_SIZE>::reference a, bool b) {
    a = a && b;
    return a;
}

bool IsWinningMove(const Board& board, const Position& position) {
    const char candidate_id = board[position];
    const char max_index = board.GetSideLength() - 1;

    std::bitset<FIELD_SIZE> potentials;
    potentials.set();

    for (char i = 0; i <= max_index; ++i) {
        // Check the 3 lines parallel an axis
        (potentials[X_WIN]) &=
            (board[{i, position.y, position.z}] == candidate_id);
        potentials[Y_WIN] &=
            (board[{position.x, i, position.z}] == candidate_id);
        potentials[Z_WIN] &=
            (board[{position.x, position.y, i}] == candidate_id);

        // Check the two diagonals in each of the 3 planes that have an axis
        // normal to it.
        potentials[X_DIAG1] &= (board[{position.x, i, i}] == candidate_id);
        potentials[Y_DIAG1] &= (board[{i, position.y, i}] == candidate_id);
        potentials[Z_DIAG1] &= (board[{i, i, position.z}] == candidate_id);
        potentials[X_DIAG2] &=
            (board[{position.x, i, max_index - i}] == candidate_id);
        potentials[Y_DIAG2] &=
            (board[{i, position.y, max_index - i}] == candidate_id);
        potentials[Z_DIAG2] &=
            (board[{i, max_index - i, position.z}] == candidate_id);

        // Check the 3-dimensional diagonals
        potentials[CUBE_DIAG1] &= (board[{i, i, i}] == candidate_id);
        potentials[CUBE_DIAG2] &=
            (board[{i, i, max_index - i}] == candidate_id);
        potentials[CUBE_DIAG3] &=
            (board[{i, max_index - i, i}] == candidate_id);
        potentials[CUBE_DIAG4] &=
            (board[{max_index - i, i, i}] == candidate_id);
    }

    return potentials.any();
}

}  // namespace cs126ttt
