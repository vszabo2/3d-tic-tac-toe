3D Tic-Tac-Toe
===

## Summary
This is a program that allows any number of people to play 3D Tic-Tac-Toe, on a
board of any size. Each player needs a copy of the executable, the ability to
listen for incoming TCP connections, the ability to make outgoing TCP
connections, and a proper configuration file.

## Dependencies
 * openFrameworks
 * Boost Asio

## Build Instructions
On Linux, with openFrameworks installed and the OF_ROOT environment variable
pointing to your installation of openFrameworks, building is as easy as:
```sh
git clone https://github.com/CS126FA19/fantastic-finale-vszabo2.git
cd fantastic-finale-vszabo2/3DTicTacToe
make
```

The executable is generated in the bin subdirectory of 3DTicTacToe.

The debug build can be done with `make Debug`, and the tests can be built with
`make tests`.

This project was developed on Linux, with gcc 9.2.0, openFrameworks 10.1, and
boost 1.71.0. However, C++, openFrameworks, and boost are cross-platform. With
some re-generation of the build files, this project should build on any common
platform. Some special configuration will be needed on other platforms to link
to the `boost_program_options` library and to build the tests.

## Configuration File
This program requires some configuration files to run. They describe how a game
is set up. Each configuration file needs to have all of the following properties
defined:

 * sideLength
   The board will be of size sideLength * sideLength * sideLength.
   This property needs to be the same for all players.
 * playerCount
   The number of players.
   This property needs to be the same for all players.
 * playerIndex
   This player's position in the move order, a number between 0 (inclusive) and
playerCount (exclusive).
   This property needs to be different for all players.
 * myPort
   The port on which to listen for a connection from the previous player in the
order.
   This property does not need to be the same for all players.
 * nextAddress
   The address at which to try to connect to the next player in the order.
   This property does not need to be the same for all players.
 * nextPort
   The port on which to try to connect to the next player in the order.
   This property does not need to be the same for all players.

The syntax of the config file is a series of lines of the form `property =
value`. There are some example config files for playing games on one computer
in the ExampleConfigs directory.

## Usage Instructions
Each player needs to start the game program and pass as the first command-line
argument the relative or absolute path of an appropriate config file. Once all
necessary connections have been established, the board will be displayed and the
game will begin. The player whose turn it is (initially the one with playerIndex
0) will have a yellow cursor displayed in the board. This player can move the
cursor with the W, A, S, D, Q, and E keys (as indicated on the screen near the
board). When this player has moved the cursor to the desired location, they can
press the Enter key to signal this. This player's move will be recorded, and
play will pass to the next player.

The winner is the first to fill a row/column (in any of the 3
horizontal/vertical directions) or a diagonal (in any plane) with their own
marker.
