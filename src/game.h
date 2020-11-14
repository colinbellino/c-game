#if !defined(GAME_H)

struct Position
{
    int x;
    int y;
};

struct Stats
{
    float moveSpeed;
};

struct Color
{
    int r;
    int g;
    int b;
    int a;
};

struct Sprite
{
    Color color;
};

struct Components
{
    Position *position;
    Stats *stats;
    Sprite *sprite;
};

struct Entity
{
    unsigned int id;
    Components components;
};

struct PlayerInput
{
    float x;
    float y;
    bool debug1;
    bool debug2;
};

struct GameState
{
    PlayerInput playerInput;
    bool initialized;
    Entity entities[2];
};

#define GAME_H
#endif
