#!/bin/sh

mkdir build

g++ -g code/linux_pokecalc.cpp -lncurses -o build/bin/pokecalc
