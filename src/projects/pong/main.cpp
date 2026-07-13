#include <iostream>
#include "../core/Starisk.h"
#include "../core/ecs/components.h"
#include "imgui/imgui.h"
#include <cmath>

extern "C" __declspec(dllexport) void destroyProject(Starisk* s) { delete s; }


extern "C" __declspec(dllexport) Starisk* runProject(GLFWwindow* window, ImGuiContext* ctx)
{
    struct Ray {
        StarVec2D p1, p2;
        StarQuad s1, s2;
        float angle = 0.0f;

        Ray(StarVec2D startPos) : p1(startPos) {}

        float findScreenAdj(){
            float ret = (float)(StariskSettings::WINDOW_WIDTH - 10) - p1.x;
            return ret; 
        }

        void findScreenEndPoint(){
            float adj = findScreenAdj();
            float radians = angle * (3.14159265f / 180.0f); 
            float opp = adj * tan(radians);

            //float hyp = std::pow(adj, 2) + std::pow(opp, 2);
            //hyp = std::sqrt(hyp);
            //std::cout << "Hyp length: " << hyp << "\n";

            p2 = StarVec2D(p1.x + adj, p1.y + opp);      
            //p2 = StarVec2D(hyp - p1.x, hyp - p1.y);

        }

        void drawPoints(Starisk* pStar){
            
            pStar->sbm->drawLine(p1, p2, StariskSettings::WINDOW_WIDTH, StariskSettings::WINDOW_HEIGHT);

            //pStar->sbm->drawLine(StarVec2D(10.0f, 60.0f), StarVec2D(100.0f, 20.0f), StariskSettings::WINDOW_WIDTH, StariskSettings::WINDOW_HEIGHT);
        }

        void setAngle(float a){ angle = a; }
    };

    ImGui::SetCurrentContext(ctx); // sync the game DLL's ImGui to editor's context
    Starisk* s = new Starisk(window);  // ✅ allocated in DLL heap
    
    

    Entity& player = s->CreateEntity(50.0f, 100.0f, 30, 80, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    player.addComponent<KeyboardMovementComponent>();
    auto& kms = s->addSystem<KeyboardMovementSystem>(ctx);
    auto& ts = s->addSystem<TransformSystem>();
    Entity& ball = s->CreateEntity(100.0f, 100.0f, 30, 30, glm::vec4(1.0f, 0.5f, 0.3f, 1.0f));
    Ray ballRay = Ray(ball.getComponent<TransformComponent>().pos);
    ballRay.angle = 30.0f;
    Entity& player2 = s->CreateEntity(200.0f, 100.0f, 30, 80, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    //Entity& player3 = s->CreateEntity(200.0f, 20.0f, 30, 80, glm::vec4(0.5f, 0.3f, 0.8f, 1.0f));
    
    player.addComponent<ColliderComponent>("AABB");
    ball.addComponent<ColliderComponent>("AABB");
    player2.addComponent<ColliderComponent>("AABB");

    auto& cs = s->addSystem<ColliderSystem>();

    cs.SetAABBSideEffect([s](std::unique_ptr<Entity>& entity, std::unique_ptr<Entity>& other){
        
    });
    //std::cout << "BallRay x: " << ballRay.p1.x << " BallRay y: " << ballRay.p1.y << "\n";
    //std::cout << "BallRay x: " << ballRay.p2.x << " BallRay y:  " << ballRay.p2.y << "\n";
    // Entity& s1 = s->CreateEntity(10, 10, 10, 10, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    // Entity& s2 = s->CreateEntity(100, 50, 10, 10, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    //ballRay.drawPoints(s);
    
    cs.SetCustomLogic([s, &ball, &ballRay, &player, &player2]() {
        auto& ballTransform = ball.getComponent<TransformComponent>();
        
        // 1. UPDATE AND DRAW EVERY FRAME (Outside the collision check)
        ballRay.p1 = ballTransform.pos;
        ballRay.findScreenEndPoint();
        ballRay.drawPoints(s);

        // 2. CHECK COLLISION
        if(ColliderSystem::AABB(player, ball)){
            auto& playerTransform = player.getComponent<TransformComponent>();
            float playerHeight = player.quad.height;
            float playerCenterY = playerTransform.pos.y + (playerHeight / 2.0f);

            // 3. APPLY LOGIC BASED ON IMPACT POSITION
            if(ballTransform.pos.y < playerCenterY)
            {
                ballRay.setAngle(ballRay.angle + 5.0f);
                
                // You MUST move the ball away, otherwise this triggers again next frame!
                // ballTransform.velocity = StarVec2D(-2.0f, -2.0f); 
                //ballTransform.pos.x += 5.0f; // Hacky push to prevent double-collision
            }
            else
            {
                ballRay.setAngle(ballRay.angle - 5.0f);
                
                // ballTransform.velocity = StarVec2D(2.0f, 2.0f);  
                //ballTransform.pos.x += 5.0f; // Hacky push to prevent double-collision
            }
        }
    });

    return s;
}