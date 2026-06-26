#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "../../StariskExport.h"
#include "../ecs.h"
#include "../../StarVec2D.h"

class STARISK_API TransformComponent : public Component
{
public:
    StarVec2D pos;
    StarVec2D velocity;
    float speed = 2.0f;

    TransformComponent(float xpos, float ypos)
    {
        pos.x = xpos;
        pos.y = ypos;
    }

    TransformComponent(float xpos, float ypos, float speed)
    {
        pos.x = xpos;
        pos.y = ypos;
        speed = speed;
    }

};

#endif