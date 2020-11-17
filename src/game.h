#if !defined(GAME_H)

#define RAYLIB_FORCE_C_LITERAL 1

#include "raylib.h"
#include "ecs.h"

#define internal static
#define local static
#define global static

struct Position
{
    int x;
    int y;
};

struct Stats
{
    float moveSpeed;
};

struct Sprite
{
    Color color;
};

struct Components
{
    Position *position;
    Stats *stats;
    Sprite *sprite;
};

struct Entity
{
    unsigned int id;
    Components components;
};

struct PlayerInput
{
    float x;
    float y;
    bool debug1;
    bool debug2;
};

struct GameState
{
    PlayerInput playerInput;
    bool initialized;
    EcsCore *ecs;
    Entity entities[2];
};

#define GAME_H
#endif
