mkdir -p ./build/

gcc -dynamiclib ./src/game.c -o ./build/game.dylib -I ./include -L ./lib -l SDL2-2.0.0