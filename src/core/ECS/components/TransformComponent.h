#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "../ecs.h"

class TransformComponent : public Component
{
public:
    float x, y;
    float speed = 2.0f;

    TransformComponent(float xpos, float ypos)
    {
        x = xpos;
        y = ypos;
    }

    TransformComponent(float xpos, float ypos, float speed)
    {
        x = xpos;
        y = ypos;
        speed = speed;
    }

};

#endif