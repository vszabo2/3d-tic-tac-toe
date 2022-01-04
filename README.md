3D Tic-Tac-Toe
===

![screenshot with a few red, green, blue, and purple balls scattered within a
3 by 3 by 3 wire grid. 3 red balls are in a row, and the text "Player 0 has
won!" is displayed in the upper left](screenshot.png)

## Summary
This is a program that allows any number of people to play 3D Tic-Tac-Toe, on a
board of any size. Each player needs a copy of the executable, a proper
configuration file, the ability to listen for incoming TCP connections, and
the ability to make outgoing TCP connections.

## Dependencies
 * openFrameworks
 * Boost.Asio
 * Boost.Program_options

## Build Instructions
On Linux, with openFrameworks installed and the OF_ROOT environment variable
pointing to your installation of openFrameworks, building is as easy as:
```sh
git clone https://github.com/vszabo2/3d-tic-tac-toe.git
cd 3d-tic-tac-toe/3DTicTacToe
make
```

The executable is generated in the bin subdirectory of 3DTicTacToe.

The debug build can be done with `make Debug`, and the tests can be built with
`make tests`.

This project was developed on Linux, with gcc 11.1.0, openFrameworks 0.11.2, and
boost 1.78.0. However, C++, openFrameworks, and boost are cross-platform. With
some re-generation of the build files, this project should build on any common
platform. Some special configuration will be needed on other platforms to link
to the `boost_program_options` library and to build the tests.

## Configuration File
This program requires some configuration files to run. They describe how a game
is set up. Each configuration file needs to have all of the following properties
defined:

 * sideLength: 
   The board will be of size sideLength * sideLength * sideLength.
   This property needs to be the same for all players.
 * playerCount: 
   The number of players.
   This property needs to be the same for all players.
 * playerIndex: 
   This player's position in the move order, a number between 0 (inclusive) and
playerCount (exclusive).
   This property needs to be different for all players.
 * myPort: 
   The port on which to listen for a connection from the previous player in the
order.
   This property does not need to be the same for all players.
 * nextAddress: 
   The address at which to try to connect to the next player in the order.
   This property does not need to be the same for all players.
 * nextPort: 
   The port on which to try to connect to the next player in the order.
   This property does not need to be the same for all players.

The syntax of the config file is a series of lines of the form `property =
value`. There are some example config files for playing games on one computer
in the ExampleConfigs directory. `demo.sh` is provided to easily run these
examples.

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

## Networking Details
The application establishes TCP connections directly between players in a ring.
The diagram below shows an example for four players. The numbers represent the
playerIndex, and the arrows represent TCP connections.

![diagram with the numbers 0, 1, 2, and 3, with arrows from 0 to 1, 1 to 2, 2
to 3, and 3 to 0](networking.png)
