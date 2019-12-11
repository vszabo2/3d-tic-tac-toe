#pragma once

#include "cube.h"

namespace cs126ttt {

typedef Point<char> Position;

constexpr size_t MESSAGE_SIZE = sizeof(Position) + 1;  // +1 for player ID

struct GameConfig {
    unsigned short side_length;
    unsigned short player_count;
    unsigned short player_index;
    unsigned short my_port;
    std::string next_address;
    unsigned short next_port;
};

class Board : public Cube<char, char> {
   public:
    static const char EMPTY = -1;
    Board(char side_length) : Cube<char, char>(side_length) { fill(EMPTY); }
};

}  // namespace cs126ttt
