#include <sstream>
#include <string>

#include "catch.hpp"
#include "cube.h"

TEST_CASE("4x4x4 Cube") {
    vszabo2ttt::Cube<int, int> arr(4);

    SECTION("GetSideLength") { REQUIRE(arr.GetSideLength() == 4); }

    SECTION("Setting and Getting") {
        int count = 64;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    arr[{i, j, k}] = count--;
                }
            }
        }

        count = 64;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    CHECK(arr[{i, j, k}] == count);
                    count--;
                }
            }
        }
    }

    SECTION("Fill") {
        arr.fill(99);

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    CHECK(arr[{i, j, k}] == 99);
                }
            }
        }
    }
}

TEST_CASE("0x0x0 Cube") {
    // We mostly care that it doesn't crash here
    vszabo2ttt::Cube<int, int> arr(0);
    REQUIRE(arr.GetSideLength() == 0);
    arr.fill(7);
}

TEST_CASE("2x2x2 Cube of strings") {
    vszabo2ttt::Cube<std::string, char> arr(2);

    REQUIRE(arr.GetSideLength() == 2);

    for (char i = 0; i < 2; ++i) {
        for (char j = 0; j < 2; ++j) {
            for (char k = 0; k < 2; ++k) {
                std::ostringstream sstream;
                sstream << '{' << i << ',' << j << ',' << k << '}';
                arr[{i, j, k}] = sstream.str();
            }
        }
    }

    for (char i = 0; i < 2; ++i) {
        for (char j = 0; j < 2; ++j) {
            for (char k = 0; k < 2; ++k) {
                std::ostringstream sstream;
                sstream << '{' << i << ',' << j << ',' << k << '}';
                CHECK(arr[{i, j, k}] == sstream.str());
            }
        }
    }
}
