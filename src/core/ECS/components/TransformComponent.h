#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "../../StariskExport.h"
#include "../ecs.h"

class STARISK_API TransformComponent : public Component
{
public:
    float x, y, velocity = 1.0f;
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