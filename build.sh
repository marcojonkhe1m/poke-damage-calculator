#!/bin/sh

if [ ! -d "build" ]; then 
    mkdir build
fi

CommonCompilerFlags="-O0 -fbuiltin -Werror -Wall -Wno-unused-variable -DPOKECALC_INTERNAL=1 -DPOKECALC_SLOW=1 -DPOKECALC_LINUX=1 -g"

pushd build/bin
g++ -c $CommonCompilerFlags -fpic -shared -o libpokecalc.so ../../code/pokecalc.cpp 
g++ $CommonCompilerFlags ../../code/linux_pokecalc.cpp -o pokecalc -lncurses -ldl
popd
