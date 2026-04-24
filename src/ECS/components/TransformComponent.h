#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "../ecs.h"

class TransformComponent : public Component
{
public:
    float x, y;

    TransformComponent(float xpos, float ypos)
    {
        x = xpos;
        y = ypos;
    }

};

#endif