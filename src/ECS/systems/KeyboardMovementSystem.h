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

    void update(StarECSManager* sem, StarBatchManager* sbm) override
    {

        for(auto& entity : sem->entities)
        {
            if(entity->hasComponent<KeyboardMovementComponent>())
            {
                //std::cout << "detected keyboardmovementcomponent for entity with id: " << entity->getId() << "\n";

                auto& kmc = entity->getComponent<KeyboardMovementComponent>();
                auto& transform = entity->getComponent<TransformComponent>();
                if(kmc.active)
                {
                    //std::cout << "keyboardmovementcomponent set to active for entity with id: " << entity->getId() << "\n";

                    if(Keyboard::key(GLFW_KEY_W))
                    {
                        //std::cout << "W pressed" << "\n";
                        transform.y-= 1 * transform.speed;
                    }
                    if(Keyboard::key(GLFW_KEY_S))
                    {
                        //std::cout << "S pressed" << "\n";
                        transform.y+= 1 * transform.speed;
                    }
                    if(Keyboard::key(GLFW_KEY_D))
                    {
                        //std::cout << "D pressed" << "\n";
                        transform.x += 1 * transform.speed;
                    }
                    if(Keyboard::key(GLFW_KEY_A))
                    {
                        //std::cout << "A pressed" << "\n";
                        transform.x-= 1 * transform.speed;
                    }

                }
            }
            else
            {
                //std::cout << "does not have keyboardmovementcomponent" << "\n";
            }
        }
    }
    

};



#endif