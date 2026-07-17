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
        StarLine line;
        bool drawnLine = false;

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

        void drawLine(Starisk* pStar){
            if(drawnLine){
                line.updatePoints(p1, p2, pStar->sbm);
            }
            else{
                line = pStar->CreateLine(p1, p2);
                drawnLine = true;
            }

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
    auto ballRay = std::make_shared<Ray>(ball.getComponent<TransformComponent>().pos);
    ballRay->angle = 30.0f;
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
    ballRay->findScreenEndPoint();
    ballRay->drawLine(s);
    
    //std::cout << ballRay.angle << "\n";
     cs.SetCustomLogic([s, &ball, ballRay, &player, &player2]() {
        auto& ballTransform = ball.getComponent<TransformComponent>();
        
        if(ColliderSystem::AABB(player, ball)){
            auto& playerTransform = player.getComponent<TransformComponent>();
            float playerHeight = player.quad.height;
            float playerCenterY = playerTransform.pos.y + (playerHeight / 2.0f);


            if(ballTransform.pos.y > playerCenterY + 50.0f){
                ballRay->setAngle(60.f);
            }
            else if(ballTransform.pos.y > playerCenterY + 20.0f || ballTransform.pos.y < playerCenterY - 20.0f){
                ballRay->setAngle(0.0f);
            }
            else if(ballTransform.pos.y < playerCenterY - 50.0f){
                ballRay->setAngle(-60.f);
            }
            
            for(float i = 0.0f; i < 1.0f; i+=0.00000001f){

                float newBallX = ballRay->p1.x + i * (ballRay->p2.x - ballRay->p1.x);
                float newBallY = ballRay->p1.y + i * (ballRay->p2.y - ballRay->p1.y);
                ballTransform.pos = StarVec2D(newBallX, newBallY);
            }

            ballRay->findScreenEndPoint();
            ballRay->drawLine(s);
        }
    });


    return s;
}