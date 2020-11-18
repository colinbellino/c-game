#include "ecs.h"
#include "components.h"
#include "game.h"

void playerInputSystem(EcsCore *core, void *stuff)
{
    GameState *gameState = (GameState *)stuff;
    gameState->playerInput = (PlayerInput){0};

    if (IsKeyDown(KEY_LEFT))
    {
        // printf("KEY_LEFT pressed!\n");
        gameState->playerInput.x = -1;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        // printf("KEY_RIGHT pressed!\n");
        gameState->playerInput.x = 1;
    }
    if (IsKeyDown(KEY_UP))
    {
        // printf("KEY_UP pressed!\n");
        gameState->playerInput.y = -1;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        // printf("KEY_DOWN pressed!\n");
        gameState->playerInput.y = 1;
    }
    if (IsKeyReleased(KEY_F1))
    {
        // printf("KEY_F1 pressed!\n");
        gameState->playerInput.debug1 = true;
    }
    if (IsKeyReleased(KEY_F2))
    {
        // printf("KEY_F2 pressed!\n");
        gameState->playerInput.debug2 = true;
    }
}

#define UPDATE_SYSTEM_MASK ecsMask(1, COMPONENT_POSITION)
void updateSystem(EcsCore *core, void *stuff)
{
    GameState *gameState = (GameState *)stuff;
    PlayerInput playerInput = gameState->playerInput;

    for (unsigned int index = 0; index < core->maxIndex + 1; index++)
    {
        EcsEntity entity = ecsGetEntity(core, index);
        if (ecsEntityHasMask(core, entity, UPDATE_SYSTEM_MASK))
        {
            if (ecsEntityHasComponent(core, entity, COMPONENT_POSITION))
            {
                PositionComponent *position = (PositionComponent *)ecsEntityGetComponent(core, entity, COMPONENT_POSITION);
                StatsComponent *stats = (StatsComponent *)ecsEntityGetComponent(core, entity, COMPONENT_STATS);
                // printf("position -> %f|%f|%f\n", position->x, position->y, position->z);

                position->x += playerInput.x * stats->moveSpeed;
                position->y += playerInput.y * stats->moveSpeed;

                if (playerInput.debug1)
                {
                    stats->moveSpeed -= 10.0;
                }
                if (playerInput.debug2)
                {
                    stats->moveSpeed += 10.0;
                }
            }
        }
    }
}

#define RENDER_SYSTEM_MASK ecsMask(1, COMPONENT_SPRITE, COMPONENT_POSITION)
void renderSystem(EcsCore *core, void *stuff)
{
    BeginDrawing();
    ClearBackground(BLACK);

    for (unsigned int index = 0; index < core->maxIndex + 1; index++)
    {
        EcsEntity entity = ecsGetEntity(core, index);
        if (ecsEntityHasMask(core, entity, RENDER_SYSTEM_MASK))
        {
            PositionComponent *position = (PositionComponent *)ecsEntityGetComponent(core, entity, COMPONENT_POSITION);
            SpriteComponent *sprite = (SpriteComponent *)ecsEntityGetComponent(core, entity, COMPONENT_SPRITE);
            // printf("sprite -> %f|%f|%f\n", sprite->x, sprite->y, sprite->z);

            DrawRectangle(
                position->x, position->y,
                20, 20,
                sprite->color);
        }
    }

    EndDrawing();
}