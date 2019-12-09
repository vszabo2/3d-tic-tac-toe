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

## 11/26/19
One of the things I considered today was having a distributed architecture for
multiplayer purposes. The way this would work theoretically is that there would
be a central server on the Internet that would help people find others to play
with. Clients would connect to it and specify the kind of game (board size)
they were looking for. Once enough players made their interests known, the
server would give each player that signed up for a particular game information
on how to connect with each other, as well as the order they would move in. The
clients would then drop the connection to the central server, listen for a
connection from the player before them, and try to establish a connection with
the player after them. The last player to move would connect with the first.
This way, data can be passed around a circle to all players. Whenever someone
made a move, they would send a message to the next player indicating who it was
that made the move and the board location where they moved. Each player would
use this information to update their view of the board and then pass the message
along to the next player, until the message gets to the player who moves before
the player that made the move the message is about, who would stop forwarding
the message. After finding out that a particular player made a move, the player
in the order after the player who made the move would know it is their turn to
make a move, and play would continue thus. For this project, I would code only
the software that the players run that connects in a circle, displays the board,
and takes and sends input. This program would take in a message that simulates
a message from the central server.

This novel architecture has some advantages and disadvantages, compared with the
architecture in which each player connects to one server that passes messages
between them. Advantages include having fewer components to code (only one
application is necessary, as opposed to a client app and server app),
better scalability (the central server doesn't have to manage all of the games
constantly, only set up the connections), and fewer single points of failure
(#players as compared to #players + 1). Disadvantages include the
impracticability of peer-to-peer connections over the Internet and the ability
of a malicious player to falsely report another player's move (although there
are ways to solve this and although the centralized architecture is not
inherently secure either). The main advantage of the decentralized idea is that
I would not need to create a separate server application, but creating a server
app that manages one game of TicTacToe is not difficult. I will continue with
the client-server architecture.

I also considered the impact of having an arbitrary number of dimensions on my
project. I would need to implement the visualization of the board separately for
each individual number of dimensions - I am not in a position to create a
scalable way to display a board of an arbitrary number of dimensions. Although
I have ideas about how to do this, they are not very good - the payoff of
implementing them would be small. So, I will focus on the three-dimensional
game.

This is how the communication will work:
 1. The server app is started and the game is configured (board side length
    and number of players). The app listens for incoming connections.
 2. The client app is started and the user inputs the connection information of
    the server. The client connects to the server. The server sends the board
    side length to the client as well as a player id that is unique in the game.
 3. When as many clients have connected as is specified, the server begins the
    game.
 4. The server tells each client the player id of the first client that is to
    make a move.
 5. That client sends the server its own player id and the position of the move
    it wishes to make.
 6. The server sends to all clients the entire board.
 7. The server sends to all clients the player id of the next player, whose turn
    it is to move.
 8. Repeat from number 5.

## 11/27/19
I have realized that ofxTalky has a number of faults. It is not possible for a
server with multiple clients to know which client sent any message, nor to send
a message to only one client. Also, it cannot receive messages that take more
than one packet. Combined with the other issues I have found and corrected with
this library, I consider ofxTalky a low-quality library. I do not want to
continue using it.

I will need to mostly start over, since the work I have done so far is specific
to ofxTalky. I do not think that I would lose so much progress as would make it
impossible for me to finish on time. Seeing as I'm starting over, I am
reconsidering my choice of architecture. Now, I think that the distributed
architecture is preferable. In the distributed architecture, I would only need
to pass one type of message, which contains exactly as much information as is
necessary. This message would include the ID of the player who made a particular
move and the coordinates of the move. Since I am fixing the number of dimensions
to 3 for now, this message is very easy to encode. The message has a length of
4 bytes, of which the first is the player id, and the remaining 3 are the
coordinates, in order. These messages will be sent to all players (indirectly).
The play can advance to the next player based on this message: when a client
receives a message that the player just before them in the order has made a
move, they know that it is their turn to make a move.

Each player needs to know their own ID, the ID of the player before them and the
player after them. We will assign ID 0 to the first player, ID 1 to the player
that moves after the first player, and so on. So, if each player knows their own
ID and the number of players, they know all the IDs necessary.

To set up this game, each player needs to know how to connect to the player that
comes after them in the order. This information would be provided by the central
server. I will use a local configuration file for each client that simulates a
message from the central server that would be sent when the game is ready to
begin. It needs to include the side length of the 3D cube, the ID of the client,
the number of players, and at least the connection information of the next
player in the order. To simplify things, I will put the connection information
of all the players in the local configuration file that I am using for now, so
that the only difference between the files given to each player is the player
ID, for ease of setup.

What library will I use for networking, instead of ofxTalky? I could use system
functions directly, but this is not portable and not very easy to code. I could
use ofxNetwork, but that is poorly-documented and geared more toward textual
messages. I just want to send 4 bytes, which should fit in one TCP packet, so I
can expect to receive it all at once. I can use the boost asio library for my
networking needs. Boost also has a different library (Program Options) that I
can use for reading the configuration file.

The application will be in one of the three states Setup, Move, and Wait at all
times. The initial state is Setup. In this state, the application is working to
establish connections with the previous and next players in the order. When this
state is entered, asynchronous operations for accepting a connection from the
previous player and making a connection to the next player are started. This
state is exited when there is a connection with both the next and previous
player. The next state depends on whether the player ID is 0. If the player ID
is 0, the next state is Move; otherwise, it is Wait. In the Move state, a cursor
is displayed in the board, which can be moved around using the keyboard. When
the user accepts the selection, a message is sent to the next player and the
state transitions to Wait. In the Wait state, the application receives messages
from the previous player, updates the board accordingly, and passes the
message on, unless its player ID matches that of the next player. If the message
has the player ID of the previous player, the application transitions to the
Move state.

## 11/29/19
Since I'm limiting my project to 3 dimensions, I don't need the n-dimensional
array I created earlier. Knowing the number of dimensions at compile time has
the great advantage of being able to use static memory for the coordinate. This
means that repeated element access from the draw() method is faster. Right now,
I'm implementing this specifically for three dimensions. This is sufficient for
my application. I could make the class template take as a parameter the number
of dimensions to be able to reuse this code for other applications, but there is
no practical reason for me to do this, since I'm only dealing with 3.

I started implementing the 3D view of the board today. I render a 3D scene
through an ofEasyCam. Using ofEasyCam means that I can rotate the view and
zoom using the mouse with no extra code. I draw (n+1)^3 lines to make an n*n*n
grid. One corner of the grid is at (0, 0, 0), and the grid is drawn in the first
octant. Each move/player marker is represented by an opaque sphere with diameter
60% of the side length of the cubical slots. The move location selection cursor
is represented by a translucent sphere with diameter 80% of the side length of
the cubical slots.

I am able to set the initial viewpoint of the camera using a precise sequence of
calls on the object during setup(). This gave me a lot of trouble, since other
orders of these calls make the camera look from a strange vantage point without
much information as to why. Some recent updates to the ofEasyCam on GitHub may
address these issues, but there has not been a release of openFrameworks since
these changes have been made.

I call ofEnableDepthTest() and ofEnableBlendMode(OF_BLENDMODE_ALPHA) in setup(),
to tweak some settings of the renderer to try to make the view look better.
ofEnableDepthTest() makes the lines that are in front of an object that is
inside the grid appear in front of it, and the lines that are behind it appear
behind it. Otherwise, the lines will either all be in front of the object or all
behind. ofEnableBlendMode(OF_BLENDMODE_ALPHA) makes it sometimes possible to see
objects that are behind a partially transparent object. The order in which draw
calls are made affects which objects can be seen through which. There are some
other slightly glitchy things with translucent objects, too.

## 11/30/19
The different states have differences in what needs to be drawn. So, I have a
separate draw function for each state. I have a pointer-to-member that points to
the draw function corresponding to the current state, and when the application
switches states, that pointer is changed. The body of the draw() function itself
simply calls the active draw function on the current object.

Today I added support for reading the config file to initialize the game. A
config file for a 1-player game looks like this:
```
sideLength = 4
playerCount = 1
playerIndex = 0

myPort = 1025
nextAddress = 127.0.0.1
nextPort = 1025
```
I use the boost program_options library to parse this file. I have a struct that
includes fields for all of the data I need to get from the config file. I fill
such a struct in the main function, before initializing the app. Then, I pass
the struct to the constructor of ofApp when I create an instance and run it. The
path to the config file, absolute or relative to the current directory, is
specified as the only command line argument. I decided to specify the
information only about the next connection, because it is not easy to load
unknown options from a config file. The only negative to this is that I have to
put slightly more work into generating the different config files by hand.
However, in my vision for how this game would be done professionally, the
central server would be doing this work, which is not difficult.

Today I also implemented the networking part of the project, using boost::asio.
In my ofApp, I have members for an io_context (to which all io operations are
tied), an acceptor (which is essentially the listening part of the app), and two
sockets (one to connect to the previous player's app and one to connect to the
next player's app. I also store an endpoint object, which gives the instructions
on how to connect to the next player (so that I can retry creating the outgoing
connection without having to re-create the endpoint object), two streambufs
(through which read and write operations are done), and a boolean which keeps
track of the state of the outgoing connection. Also, there are a couple of
std::string members on the class, which I can set based on the connection state
and which I display during the Setup state. I have a couple of class members
which have operator() defined, which call the onConnect and onRead functions on
the instance of ofApp.

The onAccept, onConnect, and onRead functions handle asynchronous I/O events.
They are responsible for changing the state of the app upon certain conditions.
They change the pointer-to-member which tells the app which draw method to call,
and they start the next asynchronous I/O operation that needs to be done.

## 12/4/19
Today I finalized how I will perform automated tests of my code. First, I tried
having a separate openFrameworks project that would include the directory
containing all the code and then exclude the main.cpp that runs the app. There
would be a main.cpp in the test project that would run the tests through Catch.
This did not work, since it turns out that the openFrameworks makefile does not
allow for the exclusion of specific files, only directories. I considered making
a small edit to the openFrameworks makefile to make this possible, but I
realized that no one else would be able to run my tests without modifying their
openFrameworks environment. There would be no way for me to run tests using
standard openFrameworks without having the main project's main.cpp in its own
directory, so I put it in its own directory. It also turns out that
openFrameworks looks for source files only in the src directory of the project,
which is generally expected, but could not be automatically assumed when working
with openFrameworks. So, I can have another directory alongside src that
contains the source files of my tests. When I want to create my test executable,
I just need to include the test directory and exclude the src/main directory
which contains the application main.cpp. To implement this, I created a new
make target in config.make, called tests, which does exactly that, as well as
setting APPNAME so that it creates a distinct executable. Creating the
application executable is done using an unmodified make Debug or make Release.

I also wrote tests today. One of the only testable portions of my code is the
Cube class. I wrote tests of all the public methods of Cube for one instance. I
also tested some methods using different template parameters and constructor
parameters, including the edge case of 0 side length.

## 12/5/19
Today I added a way for the user to input a move. It uses the w, a, s, d, q, and
e keys to move a cursor. I chose these keys due to their popularity in
different games. The way the w, a, s, and d keys are laid out on the keyboard
suggests a natural way to use them for moving something in two dimensions. The
q and e keys are nearby and cause the cursor to move along the other dimension.
The final selection is accepted using the enter key. This layout of keys seems
intuitive, at least to me. It would definitely be helpful to have some way for
the user to see which key moved the cursor in each direction at any point in
time, or to see which way "up" is, so as not to get disoriented.

I implemented this in the keyPressed function of ofApp. I make sure that the
keys have no effect unless the current state is the Move state, by testing the
active_draw_ variable. The letter keys described above modify the attributes of
the cursor_position_ variable, and I make sure that the cursor cannot be moved
out of the board. The logic I use for this part depends on the members of
cursor_position_ to be signed types. Unless there is already a marker in the
current cursor position, pressing the return key updates the current player's
view of the board and sends a message to the next player to update all of the
other players' views. I may consider having some visual feedback if the user
presses enter where there is already a marker instead of just doing nothing.

## 12/7/19
There is currently a problem with the networking code. The first player can send
a move to the second player, the second player will receive the move, and the
second player and the player before the first player can send a move, but the
first player will not receive the move of the player before them. I noticed this
problem when I tried to set up a game of two players. I figured out where the
error was by creating a two player game whose port numbers did not line up
exactly, and I used an external program to send and receive (forward) the TCP
stream between the two games. So, I was able to pinpoint the error to the
receiving side of the first player.

I double-checked my logic which starts the asynchronous read operation from
sock_prev_ after the first player presses enter. I use the same two lines of
code any time I start an asynchronous read throughout the project. (Maybe I
should consider making those two lines of code into a function.) I also defined
the macro BOOST_ASIO_ENABLE_HANDLER_TRACKING, which shows debugging information
about asynchronous operations in boost asio. I could see that the async read
operation was being started, but was never receiving any data.

To fix this issue, I had to debug into the boost code through the call to
io_context_.poll() in the update function. The read operation itself and the
dispatching of the handler occur in this function. I noticed that there were no
system calls being done during poll, since a member variable `stopped_` was set
to true by the time the first player was ready to read a move from the player
before them. It turns out that an io_context / scheduler is stopped when there
are no asynchronous operations pending. There are no asynchronous operations
pending during the time when the user enters their move. `stopped_` is set to
false initially and when a call to the restart function is made. The fix was to
call restart when the async read operation is started when enter is pressed
after a move is entered.

After I fixed this issue, another one arose. In a two-player game, the second
move of the first player would not appear on the second player's screen. This is
because the first player sent both its first and second moves when it was
supposed to send just its second move. The second player received the first
player's first move and the switched to the Move state, not reading the second
player's second move. This issue is caused by not resetting the buffer used to
send moves out of the first player. I fixed this issue by calling the consume
function on this buffer after the send operation completes.

Another change I made was to remove a remove a redundant if condition in onRead
that was left over from during development, when I had a different structure to
this function. I also set the TCP no delay option, which should help send
messages as soon as possible (without waiting for a large enough message), after
establishing a connection instead of just before.
