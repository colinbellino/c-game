#include <SDL2/SDL.h>

#include "main.h"
#include "entities.cpp"

extern "C" GAME_INIT(gameInit)
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;

    GameInitResult result = {};

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        result.errorCode = 1;
        return result;
    }

    result.window = SDL_CreateWindow(
        "Hello",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
    if (result.window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        result.errorCode = 1;
        return result;
    }

    SDL_SetWindowOpacity(result.window, 0.5f);
    SDL_CreateRenderer(result.window, -1, SDL_RENDERER_ACCELERATED);

    return result;
}

void renderPlayerSystem(SDL_Window *window, GameState *gameState)
{
    size_t entitiesCount = sizeof(gameState->entities) / sizeof(gameState->entities[0]);

    SDL_Renderer *renderer = SDL_GetRenderer(window);
    SDL_RenderClear(renderer);

    for (int entityIndex = 0; entityIndex < entitiesCount; entityIndex++)
    {
        Entity entity = gameState->entities[entityIndex];

        SDL_Rect rect;
        rect.x = entity.components.position->x;
        rect.y = entity.components.position->y;
        rect.w = 20;
        rect.h = 20;

        SDL_SetRenderDrawColor(
            renderer,
            entity.components.sprite->color.r,
            entity.components.sprite->color.g,
            entity.components.sprite->color.b,
            entity.components.sprite->color.a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    SDL_RenderPresent(renderer);
}

void movePlayerSystem(SDL_Window *window, GameState *gameState)
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

extern "C" GAME_UPDATE(gameUpdate)
{
    gameState->playerInput = (PlayerInput){0};

    if (!gameState->initialized)
    {
        Entity player = {};
        addComponentPosition(&player, 0, 0);
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

    // EntityId entityId;
    // createEntity(entityId);
    // printEntity(entityId);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return 1;
        }

        if (event.type == SDL_KEYDOWN)
        {
            SDL_KeyboardEvent keyboardEvent = event.key;
            if (keyboardEvent.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                return 1;
            }

            if (keyboardEvent.keysym.scancode == SDL_SCANCODE_LEFT)
            {
                gameState->playerInput.x = -1;
            }
            if (keyboardEvent.keysym.scancode == SDL_SCANCODE_RIGHT)
            {
                gameState->playerInput.x = 1;
            }
            if (keyboardEvent.keysym.scancode == SDL_SCANCODE_UP)
            {
                gameState->playerInput.y = -1;
            }
            if (keyboardEvent.keysym.scancode == SDL_SCANCODE_DOWN)
            {
                gameState->playerInput.y = 1;
            }
            if (keyboardEvent.keysym.scancode == SDL_SCANCODE_F1)
            {
                gameState->playerInput.debug1 = true;
            }
            if (keyboardEvent.keysym.scancode == SDL_SCANCODE_F2)
            {
                gameState->playerInput.debug2 = true;
            }
        }
    }

    renderPlayerSystem(window, gameState);
    movePlayerSystem(window, gameState);

    return 0;
}