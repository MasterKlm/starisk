#ifndef KEYBOARDMOVEMENTCOMPONENT_H
#define KEYBOARDMOVEMENTCOMPONENT_H
#include "../ecs.h"
#include "../../StariskExport.h"


class STARISK_API KeyboardMovementComponent : public Component
{
public:
    bool active = true;

};

#endif