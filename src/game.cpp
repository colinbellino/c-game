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

    SDL_SetWindowOpacity(result.window, 0.8f);
    SDL_CreateRenderer(result.window, -1, SDL_RENDERER_ACCELERATED);

    return result;
}

extern "C" GAME_UPDATE(gameUpdate)
{
    if (!gameState->initialized)
    {
        Entity player = {};
        addComponentPosition(&player, 0, 0);
        addComponentStats(&player, 10.0);
        gameState->player = player;

        gameState->initialized = true;
    }

    Position *playerPosition = gameState->player.components.position;
    Stats *playerStats = gameState->player.components.stats;

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
                playerPosition->x -= 1 * playerStats->moveSpeed;
            }
            else if (keyboardEvent.keysym.scancode == SDL_SCANCODE_RIGHT)
            {
                playerPosition->x += 1 * playerStats->moveSpeed;
            }
            else if (keyboardEvent.keysym.scancode == SDL_SCANCODE_UP)
            {
                playerPosition->y -= 1 * playerStats->moveSpeed;
            }
            else if (keyboardEvent.keysym.scancode == SDL_SCANCODE_DOWN)
            {
                playerPosition->y += 1 * playerStats->moveSpeed;
            }
            else if (keyboardEvent.keysym.scancode == SDL_SCANCODE_F1)
            {
                playerStats->moveSpeed -= 10.0;
            }
            else if (keyboardEvent.keysym.scancode == SDL_SCANCODE_F2)
            {
                playerStats->moveSpeed += 10.0;
            }
        }
    }

    SDL_Renderer *renderer = SDL_GetRenderer(window);

    SDL_RenderClear(renderer);

    SDL_Rect rect;
    rect.x = playerPosition->x;
    rect.y = playerPosition->y;
    rect.w = 20;
    rect.h = 20;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderPresent(renderer);

    return 0;
}