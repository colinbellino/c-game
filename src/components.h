#include "raylib.h"

enum ComponentType
{
    COMPONENT_POSITION,
    COMPONENT_SPRITE,
    COMPONENT_STATS,
    COMPONENT_COUNT
};

struct PositionComponent
{
    float x;
    float y;
    float z;
};

struct SpriteComponent
{
    Color color;
};

struct StatsComponent
{
    float moveSpeed;
};