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

void addComponentSprite(Entity *entity, Color color)
{
    entity->components.sprite = (Sprite *)malloc(sizeof(Sprite));
    entity->components.sprite->color = color;
}

void freeEntity(Entity *entity)
{
    if (entity->components.position)
    {
        free(entity->components.position);
    }
}