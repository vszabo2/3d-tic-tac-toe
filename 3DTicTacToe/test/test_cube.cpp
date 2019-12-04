#include "catch.hpp"
#include "cube.h"

TEST_CASE("4x4x4 Cube") {
    Cube<int, int> arr(4);

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
