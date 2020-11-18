#if !defined(MAIN_H)

#include <stdio.h>
#include <cstdint>

#define kilobytes(value) ((value)*1024LL)
#define megabytes(value) (kilobytes(value) * 1024LL)
#define gigabytes(value) (megabytes(value) * 1024LL)
#define terabytes(value) (gigabytes(Value) * 1024LL)

struct DebugReadFileResult
{
    uint32_t contentSize;
    void *content;
};

#define DEBUG_PLATFORM_READ_FROM_FILE(name) DebugReadFileResult name(const char *filename)
typedef DEBUG_PLATFORM_READ_FROM_FILE(DebugPlatformReadFromFile);

#define DEBUG_PLATFORM_WRITE_TO_FILE(name) bool name(const char *filename, uint32_t memorySize, void *memory)
typedef DEBUG_PLATFORM_WRITE_TO_FILE(DebugPlatformWriteToFile);

struct GameMemory
{
    bool isInitialized;

    uint64_t permanentStorageSize;
    void *permanentStorage; // NOTE(casey): REQUIRED to be cleared to zero at startup

    uint64_t transientStorageSize;
    void *transientStorage; // NOTE(casey): REQUIRED to be cleared to zero at startup

    DebugPlatformReadFromFile *readFromFile;
    DebugPlatformWriteToFile *writeToFile;
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
