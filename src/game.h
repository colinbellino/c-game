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

struct Components
{
    Position *position;
    Stats *stats;
};

struct Entity
{
    unsigned int id;
    Components components;
};

struct GameState
{
    Entity player;
    bool initialized;
};

#define GAME_H
#endif
