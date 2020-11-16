#include <stdlib.h>
#include <uuid/uuid.h>

#include "game.h"

typedef uuid_t EntityId;

void createEntity(EntityId &id)
{
    uuid_generate_random(id);
}

void printEntity(EntityId id)
{
    char idStr[37];
    uuid_unparse_lower(id, idStr);
    printf("Entity: %s\n", idStr);
}

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