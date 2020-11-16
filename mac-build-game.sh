mkdir -p ./build/

gcc ./src/game.cpp -o ./build/game.dylib -dynamiclib -I ./include -L ./lib -l SDL2-2.0.0 -l raylib
