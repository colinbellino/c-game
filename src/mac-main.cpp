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
        int permanentStorageSize = megabytes(64);
        memory.permanentStorage = malloc(permanentStorageSize);
        memory.permanentStorageSize = permanentStorageSize;

        memory.isInitialized = true;
    }

    if (game.init() > 0)
    {
        quit = true;
    }

    while (!quit)
    {
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