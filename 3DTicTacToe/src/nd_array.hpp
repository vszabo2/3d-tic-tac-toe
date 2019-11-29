template <typename T>
NdArray<T>::NdArray(int side_length, int num_dimensions) {
    side_length_ = side_length;
    num_dimensions_ = num_dimensions;
    data_.assign(pow(side_length, num_dimensions), T());
}

template <typename T>
T& NdArray<T>::operator[](std::vector<int> coordinates) {
    if (coordinates.size() != num_dimensions_) {
        throw std::range_error(
            "Number of coordinates is different from number of dimensions.");
    }

    int index = 0;
    int place_value = 1;
    for (int coordinate : coordinates) {
        if (coordinate < 0 || coordinate >= side_length_) {
            throw std::range_error("Coordinate is out of range.");
        }
        index += coordinate * place_value;
        place_value *= side_length_;
    }

    return data_[index];
}
