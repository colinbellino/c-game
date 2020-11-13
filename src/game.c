#include <SDL2/SDL.h>

#include "main.h"

GAME_INIT(Init)
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;

    GameInitResult result = {};

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        result.ErrorCode = 1;
        return result;
    }

    result.Window = SDL_CreateWindow(
        "Hello",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (result.Window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        result.ErrorCode = 1;
        return result;
    }

    SDL_CreateRenderer(result.Window, -1, SDL_RENDERER_ACCELERATED);

    return result;
}

GAME_UPDATE(Update)
{
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
        }
    }

    SDL_Renderer *renderer = SDL_GetRenderer(window);

    SDL_RenderClear(renderer);

    SDL_Rect rect;
    rect.x = 250;
    rect.y = 150;
    rect.w = 200;
    rect.h = 200;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderPresent(renderer);

    return 0;
}