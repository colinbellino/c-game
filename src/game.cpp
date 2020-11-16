#define RAYLIB_FORCE_C_LITERAL 1
#include <raylib.h>

#include "main.h"
#include "entities.cpp"

extern "C" GAME_INIT(gameInit)
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello raylib");
    SetTargetFPS(60);

    GameInitResult result = {};
    result.errorCode = 0;

    return result;
}

void bootstrapGame(GameState *gameState)
{
    if (!gameState->initialized)
    {
        Entity player = {};
        addComponentPosition(&player, 50, 50);
        addComponentStats(&player, 10.0);
        addComponentSprite(&player, (Color){255, 0, 0, 0});
        gameState->entities[0] = player;

        Entity player2 = {};
        addComponentPosition(&player2, 400, 400);
        addComponentStats(&player2, 10.0);
        addComponentSprite(&player2, (Color){0, 255, 0, 0});
        gameState->entities[1] = player2;

        gameState->initialized = true;
    }
}

extern "C" GAME_UPDATE(gameUpdate)
{
    bootstrapGame(gameState);
    movePlayerSystem(gameState);
    playerInputSystem(gameState);
    renderSystem(gameState);

    return WindowShouldClose();
}