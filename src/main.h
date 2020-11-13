#if !defined(MAIN_H)

#include <stdio.h>
#include <SDL2/SDL.h>

#define internal static
#define local static
#define global static

typedef struct Float2
{
    float X;
    float Y;
} Float2;

typedef struct GameState
{
    int playerX;
    int playerY;
    float playerSpeed;
} GameState;

typedef struct GameInitResult
{
    SDL_Window *Window;
    int ErrorCode;
} GameInitResult;

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
