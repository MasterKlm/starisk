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
        float t = 0.0f; 
        bool moving = false;

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
    auto& cs = s->addSystem<ColliderSystem>();
    auto& ts = s->addSystem<TransformSystem>();
    auto& kms = s->addSystem<KeyboardMovementSystem>(ctx);
    Entity& ball = s->CreateEntity(StariskSettings::WINDOW_WIDTH/2, StariskSettings::WINDOW_HEIGHT/3, 30, 30, glm::vec4(1.0f, 0.5f, 0.3f, 1.0f));
    auto ballRay = std::make_shared<Ray>(ball.getComponent<TransformComponent>().pos);
    ballRay->angle = 120.0f;
    Entity& player2 = s->CreateEntity(StariskSettings::WINDOW_WIDTH-50.0f, StariskSettings::WINDOW_HEIGHT/1.7, 30, 80, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    //Entity& player3 = s->CreateEntity(200.0f, 20.0f, 30, 80, glm::vec4(0.5f, 0.3f, 0.8f, 1.0f));
    

    player.addComponent<ColliderComponent>("AABB");
    ball.addComponent<ColliderComponent>("AABB");
    player2.addComponent<ColliderComponent>("AABB");


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

        // --- LAUNCH: only runs once per collision, not every frame ---
        if(ColliderSystem::AABB(player, ball) && !ballRay->moving){
            auto& playerTransform = player.getComponent<TransformComponent>();
            float playerHeight = player.quad.height;
            float playerCenterY = playerTransform.pos.y + (playerHeight / 2.0f);

            if(ballTransform.pos.y > playerCenterY + 50.0f){
                ballRay->setAngle(60.f);
            }
            else if(ballTransform.pos.y < playerCenterY - 50.0f){
                ballRay->setAngle(-60.f);
            }

            ballRay->p1 = ballTransform.pos;
            ballRay->findScreenEndPoint();

            // direction from p1 to p2, normalized, scaled to a speed
            float dx = ballRay->p2.x - ballRay->p1.x;
            float dy = ballRay->p2.y - ballRay->p1.y;
            float len = std::sqrt(dx * dx + dy * dy);

            if(len > 0.0001f){
                float speed = 4.0f; // tune this for travel speed
                ballTransform.velocity = StarVec2D((dx / len) * speed, (dy / len) * speed);
            }

            ballRay->moving = true;
            ballRay->drawLine(s);
        }

        // --- IN-FLIGHT: runs every frame while the ball is traveling ---
        if(ballRay->moving){
            float remainingX = ballRay->p2.x - ballTransform.pos.x;
            float remainingY = ballRay->p2.y - ballTransform.pos.y;
            float distRemaining = std::sqrt(remainingX * remainingX + remainingY * remainingY);

            if(distRemaining < 4.0f){ // close enough to p2, stop
                ballTransform.velocity = StarVec2D(0.0f, 0.0f);
                ballTransform.pos = ballRay->p2; // snap exactly to end point
                ballRay->moving = false;
            }

            ballRay->drawLine(s);
        }
    });

    return s;
}