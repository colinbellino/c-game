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

extern "C" GAME_UPDATE(gameUpdate)
{
    gameState->playerInput = (PlayerInput){0};

    if (!gameState->initialized)
    {
        Entity player = {};
        addComponentPosition(&player, 0, 0);
        addComponentStats(&player, 10.0);
        addComponentSprite(&player, (PouetColor){255, 0, 0, 0});
        gameState->entities[0] = player;

        Entity player2 = {};
        addComponentPosition(&player2, 400, 400);
        addComponentStats(&player2, 10.0);
        addComponentSprite(&player2, (PouetColor){0, 255, 0, 0});
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
    movePlayerSystem(gameState);

    return 0;
}