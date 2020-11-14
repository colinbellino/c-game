mkdir -p ./build/

hotReload=0
if [[ "$1" = "--hot-reload" ]]; then
    hotReload=1
fi

if [ $hotReload == 1 ]; then
    ./mac-build-game.sh
    gcc -Wall -g ./src/mac-main.cpp -o ./build/main -D HOT_RELOAD
else
    gcc -Wall -g ./src/mac-main.cpp -o ./build/main ./build/game.dylib -I ./include -L ./lib -l SDL2-2.0.0
fi
