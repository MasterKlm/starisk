#ifndef COLLIDERSYSTEM_H
#define COLLIDERSYSTEM_H
#include "../ecs.h"
#include "../../StariskExport.h"
#include "../components/ColliderComponent.h"
#include <functional>

class STARISK_API ColliderSystem : public System
{
    public:         
        std::function<void()> customLoop = [](){

        };

        std::function<void(std::unique_ptr<Entity>& entity, std::unique_ptr<Entity>& other)> AABBSideEffect = [](std::unique_ptr<Entity>& entity, std::unique_ptr<Entity>& other)
        {
            std::cout << "AABB collision detected between enitity (Id: " << entity->getId() << ") " << "and entity (Id: " << other->getId() << ")" << "\n";
        };
        
        void update(StarECSManager* sem, StarBatchManager* sbm) override
        {
            customLoop();
            for(auto& e : sem->entities)
            {
                if(e->hasComponent<ColliderComponent>())
                {
                    for(auto& other : sem->entities)
                    {
                        if(other == e) continue;
                        if(e->getComponent<ColliderComponent>().tag == std::string("AABB") && other->getComponent<ColliderComponent>().tag == std::string("AABB"))
                        {
                            if(ColliderSystem::AABB(e, other))
                            {
                                AABBSideEffect(e, other);
                            }
                        }
                    }
                }
            }
        }

        void SetAABBSideEffect(std::function<void(std::unique_ptr<Entity>& entity, std::unique_ptr<Entity>& other)> sideEffect)
        {
            AABBSideEffect = sideEffect;
        }

        void SetCustomLogic(std::function<void()> fn){
            customLoop = fn;
        }

        
        

        static bool AABB(std::unique_ptr<Entity>& entity, std::unique_ptr<Entity>& other)
        {
            auto& entityTransform = entity->getComponent<TransformComponent>();
            auto& otherTransform = other->getComponent<TransformComponent>();

            if(
                entityTransform.x + entity->quad.width >= otherTransform.x &&
                otherTransform.x + other->quad.width >= entityTransform.x &&
                entityTransform.y + entity->quad.height >= otherTransform.y &&
                otherTransform.y + other->quad.height >= entityTransform.y
            )
            {
                return true;
            }
            
            return false;
        }


        static bool AABB(Entity& entity, Entity& other)
        {
            auto& entityTransform = entity.getComponent<TransformComponent>();
            auto& otherTransform = other.getComponent<TransformComponent>();

            if(
                entityTransform.x + entity.quad.width >= otherTransform.x &&
                otherTransform.x + other.quad.width >= entityTransform.x &&
                entityTransform.y + entity.quad.height >= otherTransform.y &&
                otherTransform.y + other.quad.height >= entityTransform.y
            )
            {
                return true;
            }
            
            return false;
        }



};


#endif