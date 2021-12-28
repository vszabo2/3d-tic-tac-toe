#!/bin/sh

EXAMPLE_CONFIGS=$(dirname $0)
EXE=$EXAMPLE_CONFIGS/../bin/3DTicTacToe

case $1 in
    1)
        "$EXE" "$EXAMPLE_CONFIGS/1player0.ini" 2>&1 > /dev/null &
        ;;
    2)
        "$EXE" "$EXAMPLE_CONFIGS/2player0.ini" 2>&1 > /dev/null &
        "$EXE" "$EXAMPLE_CONFIGS/2player1.ini" 2>&1 > /dev/null &
        ;;
    4)
        "$EXE" "$EXAMPLE_CONFIGS/4player0.ini" 2>&1 > /dev/null &
        "$EXE" "$EXAMPLE_CONFIGS/4player1.ini" 2>&1 > /dev/null &
        "$EXE" "$EXAMPLE_CONFIGS/4player2.ini" 2>&1 > /dev/null &
        "$EXE" "$EXAMPLE_CONFIGS/4player3.ini" 2>&1 > /dev/null &
        ;;
    *)
        echo "Pass 1, 2, or 4 as the first argument to this script to run an example."
        ;;
esac
