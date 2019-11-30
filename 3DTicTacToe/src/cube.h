#pragma once

#include <algorithm>
#include <memory>
#include <boost/lambda/lambda.hpp>

template <typename CoordinateType>
struct Point {
    CoordinateType x;
    CoordinateType y;
    CoordinateType z;
};

template <typename ElementType, typename CoordinateType>
class Cube {
   private:
    std::unique_ptr<ElementType[]> data_;
    const CoordinateType side_length_;

   public:
    explicit Cube(CoordinateType side_length)
        : data_(new ElementType[side_length * side_length * side_length]),
          side_length_(side_length) {}
    ElementType& operator[](const Point<CoordinateType>& point) {
        return data_[(point.z * side_length_ + point.y) * side_length_ +
                     point.x];
    }

    void fill(ElementType value) {
        std::for_each(
            data_.get(),
            data_.get() + (side_length_ * side_length_ * side_length_),
            (boost::lambda::_1 = value));
    }

    int GetSideLength() { return side_length_; };
};
