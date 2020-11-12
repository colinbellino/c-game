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
#include "core.c"
#endif

const char *libPath = "./build/core.dylib";

typedef struct MacGameCode
{
    void *Handle;
    game_power_of_2 *PowerOf2;
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

    game_power_of_2 *powerOf2 = dlsym(handle, "PowerOf2");
    if (!powerOf2)
    {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return result;
    }

    result.Handle = handle;
    result.PowerOf2 = powerOf2;
    result.IsValid = (result.PowerOf2 != NULL);

    if (!result.IsValid)
    {
        result.PowerOf2 = gamePowerOf2Stub;
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

    gameCode->IsValid = false;
    gameCode->PowerOf2 = gamePowerOf2Stub;
}

int main()
{
    int close = 0;

    printf("-------------------------------------------------------- \n");
    printf("- Hot reload: %s. \n", (hotReload > 0 ? "enabled" : "disabled"));
    printf("-------------------------------------------------------- \n");

    MacGameCode game = macLoadGameCode();
    time_t lastModified;

    while (close == 0)
    {
        lastModified = macGetFileCreationTime(libPath);

        if (lastModified >= time(0))
        {
            printf("Reloading game code.\n");
            macUnloadGameCode(&game);
            game = macLoadGameCode();
            lastModified = time(0);
        }

        printf("-> %.2f\n", game.PowerOf2(5.0));

        usleep(300 * 1000);
    }

    return 0;
}