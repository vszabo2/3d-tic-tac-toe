#pragma once

#include "game_types.h"

namespace cs126ttt {

// Checks if the move just placed in board at position
// created a winning state.
bool IsWinningMove(const Board& board, const Position& position);

}
