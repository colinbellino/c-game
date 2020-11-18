#if !defined(GAME_H)

#define RAYLIB_FORCE_C_LITERAL 1

#include "raylib.h"
#include "ecs.h"

#define internal static
#define local static
#define global static

#define assert(expression, message)                                                    \
    if (expression == 0)                                                               \
    {                                                                                  \
        printf("Assertion failed in %s, line %d: %s.\n", __FILE__, __LINE__, message); \
        abort();                                                                       \
    }

struct PlayerInput
{
    float x;
    float y;
    bool debug1;
    bool debug2;
};

struct GameState
{
    bool isInitialized;
    PlayerInput playerInput;
    EcsCore *ecs;
    int debug;
};

#define GAME_H
#endif
