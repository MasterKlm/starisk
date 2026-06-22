#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H
#include "../ecs.h"
#include "../components/TransformComponent.h"
#include "../../StariskExport.h"

class STARISK_API TransformSystem : public System
{
public:
    //void init(StarBatchManager* sbm) override{}
    void update(StarECSManager* sem, StarBatchManager* sbm) override
    {
        
        for(auto& e : sem->entities)
        {
            if(!e->hasComponent<TransformComponent>()) continue;
            auto& tc = e->getComponent<TransformComponent>();
            tc.x *= tc.velocity;
            tc.y *= tc.velocity;
            // std::cout << "[TS] moving quad to x=" << tc.x << " y=" << tc.y << "\n";
            e->quad.move(tc.x, tc.y, sbm);
            e->quad.x = tc.x;
            e->quad.y = tc.y;
        }
    }
};

#endif