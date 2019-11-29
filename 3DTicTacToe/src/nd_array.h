#pragma once

#include <cmath>
#include <stdexcept>
#include <vector>

template <typename T>
class NdArray {
   private:
    std::vector<T> data_;
    const int side_length_;
    const int num_dimensions_;

   public:
    NdArray(int side_length, int num_dimensions);
    T& operator[](std::vector<int> coordinates);

    int GetSideLength() { return side_length_; };
    int GetNumDimensions() { return num_dimensions_; };
};

#include "nd_array.hpp"
