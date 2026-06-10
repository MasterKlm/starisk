#include "ECS/ecs.h"

ComponentId getComponentTypeID()
{
    static ComponentId lastId = 0;
    return lastId++;
}