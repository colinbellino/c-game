#include <stdlib.h>

#include "game.h"

void addComponentPosition(Entity *entity, int x, int y)
{
    entity->components.position = (Position *)malloc(sizeof(Position));
    entity->components.position->x = x;
    entity->components.position->y = y;
}

void addComponentStats(Entity *entity, float moveSpeed)
{
    entity->components.stats = (Stats *)malloc(sizeof(Stats));
    entity->components.stats->moveSpeed = moveSpeed;
}

void freeEntity(Entity *entity)
{
    if (entity->components.position)
    {
        free(entity->components.position);
    }
}