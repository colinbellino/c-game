mkdir -p ./build/

gcc -dynamiclib ./src/core.c -o ./build/core.dylib -current_version 1.0 -compatibility_version 1.0 -fPIC
# gcc -Wall -g ./src/main.c -o ./build/main ./build/core.dylib
gcc -Wall -g ./src/main.c -o ./build/main
