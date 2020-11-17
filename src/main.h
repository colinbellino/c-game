#if !defined(MAIN_H)

#include <stdio.h>
#include <SDL2/SDL.h>

#include "game.h"

struct GameInitResult
{
    SDL_Window *window;
    int errorCode;
};

#define GAME_INIT(name) GameInitResult name()
typedef GAME_INIT(GameInit);

GAME_INIT(gameInitStub)
{
    printf("gameInitStub\n");
    GameInitResult bla = {};
    return bla;
}

#define GAME_UPDATE(name) int name(SDL_Window *window, GameState *gameState)
typedef GAME_UPDATE(GameUpdate);

GAME_UPDATE(gameUpdateStub)
{
    printf("gameUpdateStub\n");
    return 0;
}

#define MAIN_H
#endif
