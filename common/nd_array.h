#ifndef ND_ARRAY_H
#define ND_ARRAY_H

#include <cmath>
#include <stdexcept>
#include <vector>

#include "ofxTalky.h"

template <typename T>
class NdArray : public TalkySerialisable {
   private:
    std::vector<T> data_;
    int side_length_;
    int num_dimensions_;

   public:
    NdArray(int side_length, int num_dimensions);
    T& operator[](std::vector<int> coordinates);

    int GetSideLength() { return side_length_; };
    int GetNumDimensions() { return num_dimensions_; };

    void serialiseToBuffer(TalkyBuffer &b) const;
    bool deSerialiseFromBuffer(TalkyBuffer &b);
};

#include "nd_array.hpp"

#endif  // ND_ARRAY_H
