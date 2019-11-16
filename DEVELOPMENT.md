# Development Log

## 11/15/19
After reading about ofxOsc some more, it does not seem very well suited for my
purposes anymore. First, it uses UDP, a protocol that is connectionless and does
not ensure that a particular message will get to the other end. Also, the
parameters of an OSC message are not named, which is an assumption I had made.
The biggest problem with OSC is that it is not bidirectional. However, I need
to pass data in both directions between the client and the server. It would be
possible using two OSC connections, one for each direction, but that is not
ideal.

Instead of ofxOsc, I can use ofxTalky. The Talky protocol is built on top of
TCP, so there is a concept of a connection between the client and server that
will be maintained, and messages sent are required to reach the other end. To
my understanding, the Talky protocol is for sending binary messages, which suits
me. I would probably use a struct to represent the information I want to send.
Talky also supports a message header which can identify the type of message to
the application. Taking advantage of this feature, I would be able to send
different kinds of messages.

## 11/16/19
Today I want to think about how to represent the state of the board. Any player
should be able to put a mark in any of the n^d locations. I will identify the
marks of each player using one byte. I will try to create a class that stores
this information in a static array. I think I will need a template for the side
length and number of dimensions. This probably won't work, because these
template values need to be known at compile time.

This works:
```cpp
#include <cmath>

template <int N, int D, typename T>
class NdArray {
    public:
        T data_[(int) pow(N, D)];
};
int main() {
    NdArray<3, 2, char> arr;
}
```

But this doesn't:
```cpp
int main() {
    int n;
    std::cin >> n;
    NdArray<n, 2, char> arr;
}
```

since non-const n cannot be used in constexpr. So, I will have to use
dynamically-allocated memory for this. I could deal with dynamic memory myself,
or I could use `std::vector`. `std::vector` has a `data` function I could use to
easily serialize it, since Talky probably won't know how to do it automatically.

I was able to successfully implement an n^d array using a one-dimensional
`std::vector` of length `pow(side_length, num_dimensions)`. Element coordinates
are given by d-length vectors. The index in the one-dimensional array is given
by `\sum_{i=0}^{d-1} n^i*v[i]`. I also implemented bounds checking for the
coordinate given for element access.
