#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "main.h"

#if !HOT_RELOAD
#include "game.cpp"
#endif

const char *libPath = "./build/game.dylib";
const char *statePath = "./build/state.out.txt";

struct MacGameCode
{
    void *handle;
    GameInit *init;
    GameUpdate *update;
    bool isValid;
};

time_t macGetFileCreationTime(const char *filePath)
{
    struct stat attrib;
    stat(filePath, &attrib);
    return attrib.st_ctime;
}

void printTime(const char *prefix, time_t time)
{
    char date[18];
    strftime(date, 18, "%d-%m-%y %H:%M:%S", gmtime(&time));
    printf("%s%s\n", prefix, date);
    date[0] = 0;
}

#if HOT_RELOAD
MacGameCode macLoadGameCode(void)
{
    MacGameCode result = {};

    void *handle = dlopen(libPath, RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "Error: %s\n", dlerror());
        return result;
    }

    result.handle = handle;
    result.init = (GameInit *)dlsym(handle, "gameInit");
    result.update = (GameUpdate *)dlsym(handle, "gameUpdate");
    result.isValid = (result.init != NULL &&
                      result.update != NULL);

    if (!result.isValid)
    {
        result.init = gameInitStub;
        result.update = gameUpdateStub;
    }

    return result;
}

void macUnloadGameCode(MacGameCode *gameCode)
{
    if (gameCode->handle)
    {
        dlclose(gameCode->handle);
        gameCode->handle = NULL;
    }

    gameCode->init = gameInitStub;
    gameCode->update = gameUpdateStub;
    gameCode->isValid = false;
}
#endif

DEBUG_PLATFORM_READ_FROM_FILE(macDebugPlatformReadEntireFile)
{
    DebugReadFileResult result = {};

    FILE *filePointer = fopen(filename, "r");
    if (filePointer == 0)
    {
        fprintf(stderr, "Failed to open file %s.\n", filename);
        return result;
    }

    fseek(filePointer, 0, SEEK_END);
    long contentSize = ftell(filePointer);
    fseek(filePointer, 0, SEEK_SET);

    char *content = (char *)malloc(contentSize + 1);
    fread(content, 1, contentSize, filePointer);

    result.content = content;
    result.contentSize = contentSize;

    printf("content -> %s\n", content);
    fclose(filePointer);

    return result;
}

DEBUG_PLATFORM_WRITE_TO_FILE(macDebugPlatformWriteEntireFile)
{
    int success = remove(filename);

    FILE *filePointer;
    filePointer = fopen(filename, "wb");

    printf("size -> %i\n", memorySize);
    int bla = fwrite(memory, memorySize, 1, filePointer);
    printf("success -> %i\n", bla);

    // char *pouet = "Hello from code?";
    // printf("size -> %i\n", strlen(pouet));
    // int bla2 = fwrite(pouet, strlen(pouet), 1, filePointer);
    // printf("bla2 -> %i\n", bla2);

    fclose(filePointer);

    return 0;
}

int main()
{
    MacGameCode game = {};

#if HOT_RELOAD
    printf("- Hot reload: enabled. \n");
    game = macLoadGameCode();
    time_t lastModified;
    time_t lastReload;
#else
    game.init = gameInit;
    game.update = gameUpdate;
    game.isValid = true;
#endif

    bool quit = false;

    GameMemory memory = {};
    if (!memory.isInitialized)
    {
        int permanentStorageSize = megabytes(2);
        memory.permanentStorage = malloc(permanentStorageSize);
        memory.permanentStorageSize = permanentStorageSize;
        memory.readFromFile = macDebugPlatformReadEntireFile;
        memory.writeToFile = macDebugPlatformWriteEntireFile;

        memory.isInitialized = true;
    }

    if (game.init() > 0)
    {
        quit = true;
    }

    while (!quit)
    {
        if (IsKeyReleased(KEY_F5))
        {
            printf("Saving game state.\n");
            memory.writeToFile(statePath, memory.permanentStorageSize, memory.permanentStorage);
        }

        if (IsKeyReleased(KEY_F8))
        {
            printf("Loading game state.\n");
            DebugReadFileResult result = memory.readFromFile(statePath);
            if (result.contentSize > 0)
            {
                memory.permanentStorage = result.content;
                // memory.permanentStorageSize = result.contentSize;
            }
        }

        quit = game.update(&memory);

#if HOT_RELOAD
        time_t now = time(0);
        lastModified = macGetFileCreationTime(libPath);
        // printTime("lastModified -> ", lastModified);
        // printTime("now -> ", now);

        if (lastModified > lastReload)
        {
            printf("Reloading game code.\n");
            macUnloadGameCode(&game);
            usleep(100 * 1000);
            game = macLoadGameCode();
            lastReload = now;
        }
#endif
    }

    return 0;
}