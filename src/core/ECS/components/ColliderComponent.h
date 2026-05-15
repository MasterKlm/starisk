#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H
#include "../ecs.h"
#include <string>
#include "TransformComponent.h"

class ColliderComponent : public Component
{
    public:

    TransformComponent* transform;
    std::string tag;

    ColliderComponent(const char* colliderSystemTag)
    {
        tag = colliderSystemTag;
    }


    void init() override{
        if(entity->hasComponent<TransformComponent>())
        {
            transform = &entity->getComponent<TransformComponent>(); 
        }
        else
        {
            std::cout << "Add a TransformComponent to entity or no collider system functions will work." << "\n";
        }
    }
};












#endif