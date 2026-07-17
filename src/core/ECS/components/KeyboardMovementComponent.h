#ifndef KEYBOARDMOVEMENTCOMPONENT_H
#define KEYBOARDMOVEMENTCOMPONENT_H
#include "../../StariskExport.h"
#include <functional>
#include "./TransformComponent.h"
#include "../../io/Keyboard.h"
#include "../../io/Mouse.h"
#include "../../io/Joystick.h"

class STARISK_API KeyboardMovementComponent : public Component
{
public:
    bool active = true;
    std::function<void()> customLogic = NULL;
};

#endif