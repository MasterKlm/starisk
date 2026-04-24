#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H
#include "../systems.h"
#include "../../io/Keyboard.h"
#include "../../io/Mouse.h"
#include "../components/KeyboardMovementComponent.h"
#include "../components/TransformComponent.h"
#include "../../io/Joystick.h"
#include "../ecs.h"
#include <functional>

class KeyboardMovementSystem : public System
{

 public:
    KeyboardMovementSystem(){}   

    void update() override
    {
        extern StarECSManager sem;

        for(auto& entity : sem.entities)
        {
            if(entity->hasComponent<KeyboardMovementComponent>())
            {
                auto kmc = entity->getComponent<KeyboardMovementComponent>();
                auto transform = entity->getComponent<TransformComponent>();
                if(kmc.active)
                {
                    if(Keyboard::key(GLFW_KEY_W))
                    {
                        transform.y++;
                    }
                    if(Keyboard::key(GLFW_KEY_S))
                    {
                        transform.y--;
                    }
                    if(Keyboard::key(GLFW_KEY_D))
                    {
                        transform.x++;
                    }
                    if(Keyboard::key(GLFW_KEY_A))
                    {
                        transform.x--;
                    }

                }
            }
        }
    }
    

};



#endif