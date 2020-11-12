mkdir -p ./build/

hotReload=0
if [[ "$1" = "--hot-reload" ]]; then
    hotReload=1
fi

./mac-build-game.sh

if [ $hotReload == 1 ]; then
    gcc -Wall -g ./src/mac-main.c -o ./build/main -D HOT_RELOAD
else
    gcc -Wall -g ./src/mac-main.c -o ./build/main ./build/core.dylib
fi
