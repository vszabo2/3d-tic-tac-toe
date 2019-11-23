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

## 11/18/19
Today I got an empty project that includes ofxTalky to compile. This was
difficult for a couple of reasons. First, ofxTalky was based on an old version
of openFrameworks, so it used an old interface for ofThread. I had to convert
the startThread and stopThread function calls to follow the new interface.
Second, ofxTalky requires Poco. I tried adding ofxPoco to addons.make, but that
caused a segfault immediately upon any call to an openFrameworks function (ie 
ofSetupOpenGL). To solve this issue, I followed the advice at
https://forum.openframeworks.cc/t/using-ofxpoco-results-into-segmentation-fault/30214
. I removed ofxPoco, I installed poco on my system, and I modified the
addon_config.mk file of ofxTalky to use the correct linker flags.

## 11/19/19
I was able to use my NdArray class in my openFrameworks project by adding 
`PROJECT_EXTERNAL_SOURCE_PATHS = ../common/` to config.make. I set up an
ofxTalky object to act as a server and send an NdArray of ints through it.

## 11/20/19
I created another openFrameworks project with an ofxTalky object acting as a
client. It connects to the server running on the local machine in the other
openFrameworks project and extracts an NdArray of ints from it.

Currently, this doesn't work well. The two int parameters get passed through
correctly, but the contents of the array does not. This is because the current
way the NdArray is being serialized is by copying its static memory, including
the ints and whatever static memory is in the std::vector. However, the bulk of
the array is stored in dynamic memory. The std::vector only has a pointer to the
data. The value of the pointer gets sent over, but not the data itself. When the
client receives the NdArray, the value of the pointer to the data in the
internal std::vector is set, but it points to a memory address in the server,
which is an invalid address in the client. This causes a segfault.

To fix this, I will implement my own serialization of the NdArray class. I will
make the class inherit from TalkySerializable and implement the functions
associated with that "interface." Besides my two int data members,
I will need to put the size of the std::vector and the memory that its data
pointer points to into the buffer.

## 11/21/19
I represent an NdArray in binary serialized form starting with the side length,
followed by the number of dimensions, followed by the data of the contents
of the array as it is represented internally in the vector, one element
followed by another. This will work as long as the elements have only static
memory. If I was interested in serializing an array that has elements that
have dynamic memory, I could serialize each one of the elements.

I implemented this by making NdArray inherit from TalkySerialisable and
implementing the serialization and deserialization functions. To take
advantage of my method of serialization instead of the default memcpy that is
used with the insertion operator, I had to cast my NdArray object to a
TalkySerialisable&. Without the cast, the old memcpy serialization is used.

I had to modify the extraction operator of TalkyMessage to be able to
incrementally deserialize the NdArray. I removed the check that the size of the
buffer is the same as the sizeof the item into which I was extracting.

## 11/23/19
Today I considered merging my two openFrameworks projects (client and server)
into one, with two separate main classes. I was able to do this by changing the
makefile and main.cpp. I added this to config.make:
```
client :
	APPNAME=client PROJECT_DEFINES=MAIN_CLASS=ClientApp make
server :
	APPNAME=server PROJECT_DEFINES=MAIN_CLASS=ServerApp make
```
And, I changed made main.cpp have this line:
```
	ofRunApp(new MAIN_CLASS());
```
I would have all of the files that are common to the two apps in the src
directory of one project, and separate ClientApp.h, ClientApp.cpp, ServerApp.h,
ServerApp.cpp.

The approach of merging the projects would make the file structure flatter and
require less switching directories. But, it uses a nonstandard makefile. This is
a major disadvantage. Not having to mess with the makefile is one of
openFrameworks' main principles. Going forward with this idea would make it
quite difficult for anyone to build my project, except if they use my makefile.

So, I will go back to having multiple openFrameworks projects, with a separate
directory outside with common files. Initially, I thought I would have my common
files not depend on openFrameworks, but there really are parts that are common
to the client and server that need openFrameworks. It will be nice to be able
to easily see which files belong to which app. Also, it will be possible to
compile my app on nearly any platform supported by openFrameworks.

I also considered deleting the .qbs files in each project directory, since I
don't use them at all. But, I decided to keep them, so that anyone who uses qbs
can easily build my project. It doesn't really hurt me to have them. However,
the openFrameworks project generator doesn't properly update them. I have to
update them myself.
decided to kee
