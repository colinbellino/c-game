#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "main.h"

#if HOT_RELOAD
int hotReload = 1;
#else
int hotReload = 0;
#include "game.c"
#endif

const char *libPath = "./build/game.dylib";

typedef struct MacGameCode
{
    void *Handle;
    GameInit *Init;
    GameUpdate *Update;
    int IsValid;
} MacGameCode;

time_t macGetFileCreationTime(const char *filePath)
{
    struct stat attrib;
    stat(filePath, &attrib);
    return attrib.st_ctime;
}

void printTime(time_t time)
{
    char date[18];
    strftime(date, 18, "%d-%m-%y %H:%M:%S", gmtime(&time));
    printf("%s\n", date);
    date[0] = 0;
}

MacGameCode macLoadGameCode(void)
{
    MacGameCode result = {};

    void *handle = dlopen(libPath, RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "Error: %s\n", dlerror());
        return result;
    }

    result.Handle = handle;
    result.Init = dlsym(handle, "Init");
    result.Update = dlsym(handle, "Update");
    result.IsValid = (result.Init != NULL &&
                      result.Update != NULL);

    if (!result.IsValid)
    {
        result.Init = gameInitStub;
        result.Update = gameUpdateStub;
    }

    return result;
}

void macUnloadGameCode(MacGameCode *gameCode)
{
    if (gameCode->Handle)
    {
        dlclose(gameCode->Handle);
        gameCode->Handle = NULL;
    }

    gameCode->Init = gameInitStub;
    gameCode->Update = gameUpdateStub;
    gameCode->IsValid = false;
}

int main()
{
    printf("--------------------------------------------------------\n");
    printf("- Hot reload: %s. \n", (hotReload > 0 ? "enabled" : "disabled"));
    printf("--------------------------------------------------------\n");

    int quit = 0;
    MacGameCode game = macLoadGameCode();
    time_t lastModified;

    GameInitResult result = game.Init();
    if (result.ErrorCode > 0)
    {
        quit = 1;
    }

    while (quit == 0)
    {
        quit = game.Update(result.Window);

        lastModified = macGetFileCreationTime(libPath);
        if (lastModified >= time(0))
        {
            printf("Reloading game code.\n");
            macUnloadGameCode(&game);
            usleep(1 * 1000); // Wait 1ms just to be safe.
            game = macLoadGameCode();
            lastModified = time(0);
        }
    }

    return 0;
}