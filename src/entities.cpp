#include <stdlib.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include <raylib.h>

#include "game.h"

typedef uuid_t EntityId;

void createEntity(EntityId &id)
{
    uuid_generate_random(id);
}

void printEntity(EntityId id)
{
    char idStr[37];
    uuid_unparse_lower(id, idStr);
    printf("Entity: %s\n", idStr);
}

void addComponentPosition(Entity *entity, int x, int y)
{
    entity->components.position = (Position *)malloc(sizeof(Position));
    entity->components.position->x = x;
    entity->components.position->y = y;
}

void addComponentStats(Entity *entity, float moveSpeed)
{
    entity->components.stats = (Stats *)malloc(sizeof(Stats));
    entity->components.stats->moveSpeed = moveSpeed;
}

void addComponentSprite(Entity *entity, Color color)
{
    entity->components.sprite = (Sprite *)malloc(sizeof(Sprite));
    entity->components.sprite->color = color;
}

void freeEntity(Entity *entity)
{
    if (entity->components.position)
    {
        free(entity->components.position);
    }
}

void renderSystem(GameState *gameState)
{
    size_t entitiesCount = sizeof(gameState->entities) / sizeof(gameState->entities[0]);

    BeginDrawing();
    ClearBackground(BLACK);

    for (int entityIndex = 0; entityIndex < entitiesCount; entityIndex++)
    {
        Entity entity = gameState->entities[entityIndex];

        DrawRectangleGradientV(
            entity.components.position->x, entity.components.position->y,
            20, 20, WHITE, entity.components.sprite->color);
    }

    EndDrawing();
}

void movePlayerSystem(GameState *gameState)
{
    size_t entitiesCount = sizeof(gameState->entities) / sizeof(gameState->entities[0]);

    for (int entityIndex = 0; entityIndex < entitiesCount; entityIndex++)
    {
        Entity entity = gameState->entities[entityIndex];

        entity.components.position->x += gameState->playerInput.x * entity.components.stats->moveSpeed;
        entity.components.position->y += gameState->playerInput.y * entity.components.stats->moveSpeed;

        if (gameState->playerInput.debug1)
        {
            entity.components.stats->moveSpeed -= 10.0;
        }
        if (gameState->playerInput.debug2)
        {
            entity.components.stats->moveSpeed += 10.0;
        }
    }
}

void playerInputSystem(GameState *gameState)
{
    gameState->playerInput = (PlayerInput){0};

    if (IsKeyPressed(KEY_LEFT))
    {
        printf("KEY_LEFT pressed!\n");
        gameState->playerInput.x = -1;
    }
    if (IsKeyPressed(KEY_RIGHT))
    {
        printf("KEY_RIGHT pressed!\n");
        gameState->playerInput.x = 1;
    }
    if (IsKeyPressed(KEY_UP))
    {
        printf("KEY_UP pressed!\n");
        gameState->playerInput.y = -1;
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        printf("KEY_DOWN pressed!\n");
        gameState->playerInput.y = 1;
    }
    if (IsKeyPressed(KEY_F1))
    {
        printf("KEY_F1 pressed!\n");
        gameState->playerInput.debug1 = true;
    }
    if (IsKeyPressed(KEY_F2))
    {
        printf("KEY_F2 pressed!\n");
        gameState->playerInput.debug2 = true;
    }
}