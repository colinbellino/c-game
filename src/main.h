#if !defined(MAIN_H)

#include <stdio.h>
#include <cstdint>

#define kilobytes(value) ((value)*1024LL)
#define megabytes(value) (kilobytes(value) * 1024LL)
#define gigabytes(value) (megabytes(value) * 1024LL)
#define terabytes(value) (gigabytes(Value) * 1024LL)

struct GameMemory
{
    bool isInitialized;

    uint64_t permanentStorageSize;
    void *permanentStorage; // NOTE(casey): REQUIRED to be cleared to zero at startup

    uint64_t transientStorageSize;
    void *transientStorage; // NOTE(casey): REQUIRED to be cleared to zero at startup

    // debug_platform_free_file_memory *DEBUGPlatformFreeFileMemory;
    // debug_platform_read_entire_file *DEBUGPlatformReadEntireFile;
    // debug_platform_write_entire_file *DEBUGPlatformWriteEntireFile;
};

#define GAME_INIT(name) int name()
typedef GAME_INIT(GameInit);

GAME_INIT(gameInitStub)
{
    printf("gameInitStub\n");
    return 0;
}

#define GAME_UPDATE(name) int name(GameMemory *memory)
typedef GAME_UPDATE(GameUpdate);

GAME_UPDATE(gameUpdateStub)
{
    printf("gameUpdateStub\n");
    return 0;
}

#define MAIN_H
#endif
