#include "main.h"
#include "game.h"
#include "ecs.h"
#include "systems.h"

extern "C" GAME_INIT(gameInit)
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello raylib");
    SetTargetFPS(60);

    return 0;
}

void bootstrapGame(GameState *gameState)
{
    if (!gameState->isInitialized)
    {
        printf("Resetting game state!\n");

        gameState->debug = 0;

        gameState->ecs = ecsCreateCore(1000, COMPONENT_COUNT, 3);
        ecsRegisterComponent(gameState->ecs, COMPONENT_POSITION, 1000, sizeof(PositionComponent), NULL);
        ecsRegisterComponent(gameState->ecs, COMPONENT_SPRITE, 1000, sizeof(SpriteComponent), NULL);
        ecsRegisterComponent(gameState->ecs, COMPONENT_STATS, 1000, sizeof(StatsComponent), NULL);

        ecsRegisterSystem(gameState->ecs, playerInputSystem, ECS_SYSTEM_UPDATE);
        ecsRegisterSystem(gameState->ecs, updateSystem, ECS_SYSTEM_UPDATE);
        ecsRegisterSystem(gameState->ecs, renderSystem, ECS_SYSTEM_RENDER);

        {
            EcsEntity playerEntity = ecsCreateEntity(gameState->ecs);
            PositionComponent position = {400, 200, 0};
            SpriteComponent sprite = {(Color){0, 0, 255, 255}};
            StatsComponent stats = {10.0};
            ecsEntityAddComponent(gameState->ecs, playerEntity, COMPONENT_POSITION, &position);
            ecsEntityAddComponent(gameState->ecs, playerEntity, COMPONENT_SPRITE, &sprite);
            ecsEntityAddComponent(gameState->ecs, playerEntity, COMPONENT_STATS, &stats);
        }

        {
            EcsEntity enemyEntity = ecsCreateEntity(gameState->ecs);
            PositionComponent position = {600, 100, 0};
            SpriteComponent sprite = {(Color){255, 0, 0, 255}};
            StatsComponent stats = {10.0};
            ecsEntityAddComponent(gameState->ecs, enemyEntity, COMPONENT_POSITION, &position);
            ecsEntityAddComponent(gameState->ecs, enemyEntity, COMPONENT_SPRITE, &sprite);
            ecsEntityAddComponent(gameState->ecs, enemyEntity, COMPONENT_STATS, &stats);
        }

        gameState->isInitialized = true;
    }
}

extern "C" GAME_UPDATE(gameUpdate)
{
    assert(sizeof(GameState) <= memory->permanentStorageSize, "GameState can't be larger than permanent storage");

    GameState *gameState = (GameState *)memory->permanentStorage;

    DrawText(TextFormat("debug: %i", gameState->debug), 20, 20, 20, WHITE);
    if (IsKeyReleased(KEY_SPACE))
    {
        printf("KEY_SPACE pressed!\n");
        gameState->debug += 1;
    }

    bootstrapGame(gameState);

    if (IsKeyPressed(KEY_ENTER))
    {
        gameState->isInitialized = false;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    ecsRunSystems(gameState->ecs, ECS_SYSTEM_UPDATE, gameState);
    ecsRunSystems(gameState->ecs, ECS_SYSTEM_RENDER, gameState);

    EndDrawing();

    return WindowShouldClose();
}