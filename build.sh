#!/bin/sh

if [ ! -d "build" ]; then 
    mkdir build
fi

pushd build/bin
g++ -Werror -Wall -Wno-unused-variable -DPOKECALC_INTERNAL=1 -DPOKECALC_SLOW=1 -DPOKECALC_LINUX=1 -g ../../code/linux_pokecalc.cpp -lncurses -o pokecalc
popd
