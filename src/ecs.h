#if !defined(ECS_H)

//
// This is just a copy of https://github.com/soulfoam/ecs/blob/master/ecs.h that i rewrote
// by myself with minor changes to understand how it works.
//

#include <cstring>
#include <stdio.h>

#include "stacks.h"

#define internal static

typedef uint64_t EcsEntity;
typedef uint32_t EcsComponentType;

typedef void (*EcsSystemFunction)(struct EcsCore *core, void *stuff);
typedef void (*EcsComponentDestroyFunction)(void *data);

#define ecsMask(componentTypeCount, ...) \
    componentTypeCount, (EcsComponentType[]) { __VA_ARGS__ }

struct EcsComponentPool
{
    void *data;
    uint32_t count;
    uint32_t size;
    EcsComponentDestroyFunction destroyFunction;
    Stack *indexes;
};

enum EcsSystemType
{
    ECS_SYSTEM_UPDATE,
    ECS_SYSTEM_RENDER
};

struct EcsSystem
{
    EcsSystemType type;
    EcsSystemFunction function;
};

struct EcsCore
{
    int maxEntities;
    int componentCount;
    int systemCount;
    Stack *indexes;
    int maxIndex;
    uint32_t *versions;
    uint32_t *components;
    bool *componentMasks;
    EcsComponentPool *componentPool;
    EcsSystem *systems;
    int systemsTop;
};

internal EcsComponentPool createComponentPool(
    uint32_t count,
    uint32_t size,
    EcsComponentDestroyFunction destroyFunction)
{
    EcsComponentPool pool;
    pool.data = malloc(count * size);
    pool.count = count;
    pool.size = size;
    pool.destroyFunction = destroyFunction;
    pool.indexes = createStack(count);

    for (uint32_t i = count; i-- > 0;)
    {
        pushStack(pool.indexes, i);
    }

    return pool;
}

internal uint32_t popComponentPool(EcsComponentPool *pool, void *data)
{
    uint32_t index = popStack(pool->indexes);
    uint8_t *ptr = (uint8_t *)((char *)pool->data + (index * pool->size));
    memcpy(ptr, data, pool->size);
    return index;
}

internal inline EcsEntity getEntityId(uint32_t index, uint32_t version)
{
    return (((uint64_t)version << 32) | index);
}

internal inline uint32_t getEntityIndex(uint64_t id)
{
    return ((uint32_t)id);
}

internal inline uint32_t getEntityVersion(uint64_t id)
{
    return ((uint32_t)(id >> 32));
}

EcsCore *ecsCreateCore(
    uint32_t maxEntities,
    uint32_t componentCount,
    uint32_t systemCount)
{
    EcsCore *core = (EcsCore *)malloc(sizeof(*core));
    core->maxEntities = maxEntities;
    core->componentCount = componentCount;
    core->systemCount = systemCount;
    core->indexes = createStack(maxEntities);
    core->maxIndex = 0;
    core->versions = (uint32_t *)malloc(maxEntities * sizeof(uint32_t));
    core->components = (uint32_t *)malloc(maxEntities * componentCount * sizeof(uint32_t));
    core->componentMasks = (bool *)malloc(maxEntities * componentCount * sizeof(bool));
    core->componentPool = (EcsComponentPool *)malloc(componentCount * sizeof(*core->componentPool));
    core->systems = (EcsSystem *)malloc(systemCount * sizeof(*core->systems));
    core->systemsTop = 0;

    for (uint32_t entityIndex = maxEntities; entityIndex-- > 0;)
    {
        pushStack(core->indexes, entityIndex);

        core->versions[entityIndex] = 0;
        for (uint32_t componentIndex = 0; componentIndex < componentCount; componentIndex++)
        {
            core->components[entityIndex * componentCount + componentIndex] = 0;
            core->componentMasks[entityIndex * componentCount + componentIndex] = 0;
        }
    }

    for (uint32_t systemIndex = 0; systemIndex < systemCount; systemIndex++)
    {
        core->systems[systemIndex].function = NULL;
    }

    for (uint32_t componentIndex = 0; componentIndex < core->componentCount; componentIndex++)
    {
        core->componentPool[componentIndex].data = NULL;
    }

    return core;
}

void ecsRegisterSystem(EcsCore *core, EcsSystemFunction function, EcsSystemType type)
{
    EcsSystem *system = &core->systems[core->systemsTop++];
    system->function = function;
    system->type = type;
}

void ecsRunSystems(EcsCore *core, EcsSystemType type, void *stuff)
{
    for (uint32_t i = 0; i < core->systemsTop; i++)
    {
        EcsSystem *system = &core->systems[i];
        if (system->type == type)
        {
            system->function(core, stuff);
        }
    }
}

void ecsRegisterComponent(
    EcsCore *core,
    EcsComponentType componentType,
    uint32_t count, uint32_t size,
    EcsComponentDestroyFunction destroyComponentFunction)
{
    if (core->componentPool[componentType].data != NULL)
    {
        printf("Registered Component type %u more than once.\n", componentType);
        return;
    }

    core->componentPool[componentType] = createComponentPool(count, size, destroyComponentFunction);
}

EcsEntity ecsCreateEntity(EcsCore *core)
{
    uint32_t index = popStack(core->indexes);
    uint32_t version = core->versions[index];

    if (index > core->maxIndex)
    {
        core->maxIndex = index;
    }

    return getEntityId(index, version);
}

EcsEntity ecsGetEntity(EcsCore *core, uint32_t index)
{
    return getEntityId(index, core->versions[index]);
}

bool ecsEntityHasComponent(EcsCore *core, EcsEntity entity, EcsComponentType componentType)
{
    return core->componentMasks[getEntityIndex(entity) * core->componentCount + componentType];
}

bool ecsEntityHasMask(
    EcsCore *core,
    EcsEntity entity,
    uint32_t componentTypeCount,
    EcsComponentType *componentTypes)
{
    for (uint32_t componentTypeIndex = 0; componentTypeIndex < componentTypeCount; componentTypeIndex++)
    {
        if (!ecsEntityHasComponent(core, entity, componentTypes[componentTypeIndex]))
        {
            return false;
        }
    }

    return true;
}

void *ecsEntityGetComponent(EcsCore *core, EcsEntity entity, EcsComponentType type)
{
    uint32_t index = getEntityIndex(entity);

    if (!ecsEntityHasComponent(core, entity, type))
    {
        printf("Trying to get non existent component %u on entity %llu (index %u)\n", type, entity, index);
        return NULL;
    }

    uint32_t componentIndex = core->components[index * core->componentCount + type];
    uint8_t *componentPointer = (uint8_t *)((char *)core->componentPool[type].data + (componentIndex * core->componentPool[type].size));
    return componentPointer;
}

void ecsEntityAddComponent(
    EcsCore *core,
    EcsEntity entity,
    EcsComponentType componentType,
    void *componentData)
{
    uint32_t index = getEntityIndex(entity);

    if (ecsEntityHasComponent(core, entity, componentType))
    {
        printf("Component %u already exists on entity %llu (index %u)", componentType, entity, index);
        return;
    }

    if (core->componentPool[componentType].data == NULL)
    {
        printf("Component %u not registered.\n", componentType);
        return;
    }

    EcsComponentPool *pool = &core->componentPool[componentType];
    uint32_t c_index = popComponentPool(pool, componentData);
    core->components[index * core->componentCount + componentType] = c_index;
    core->componentMasks[index * core->componentCount + componentType] = true;
}

#define ECS_H
#endif
