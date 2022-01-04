#pragma once

#include <exception>
#include <ios>
#include <string>

#include "cube.h"

namespace vszabo2ttt {

typedef Point<unsigned char> Position;

constexpr std::streamsize MESSAGE_SIZE =
    sizeof(Position) + 1;  // +1 for player ID

struct GameConfig {
    unsigned short side_length;
    unsigned short player_count;
    unsigned short player_index;
    unsigned short my_port;
    std::string next_address;
    unsigned short next_port;
};

class Board : public Cube<unsigned char, unsigned char> {
   public:
    static constexpr unsigned char EMPTY = -1;
    Board(unsigned char side_length)
        : Cube<unsigned char, unsigned char>(side_length) {
        fill(EMPTY);
    }
};

class InvalidPositionException : public std::exception {
   public:
    virtual const char* what() const noexcept {
        return "The requested move is in a position that has already been "
               "taken.";
    }
};

}  // namespace vszabo2ttt
