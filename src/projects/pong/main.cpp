#include <iostream>
#include "../core/Starisk.h"
#include "../core/ecs/components.h"
#include "imgui/imgui.h"

extern "C" __declspec(dllexport) void destroyProject(Starisk* s) { delete s; }


extern "C" __declspec(dllexport) Starisk* runProject(GLFWwindow* window, ImGuiContext* ctx)
{
    struct Ray {
        StarVec2D p1, p2;
        StarQuad s1, s2;
        float angle = 0.0f;

        Ray(StarVec2D startPos) : p1(startPos) {}

        float findScreenAdj(){
            float ret = p1.x + StariskSettings::WINDOW_WIDTH - 10;
            return ret; 
        }

        void findScreenEndPoint(){
            float adj = findScreenAdj();  
            float hyp = adj * cos(angle);
            p2 = StarVec2D(p1.x + hyp, p1.y + hyp);
        }

        void drawPoints(Starisk* pStar, Entity* s1, Entity* s2){
            s1 = &pStar->CreateEntity(p1.x, p1.y, 10.0f, 10.0f);
            s2 = &pStar->CreateEntity(p2.x, p2.y, 10.0f, 10.0f);
        }
    };

    ImGui::SetCurrentContext(ctx); // sync the game DLL's ImGui to editor's context
    Starisk* s = new Starisk(window);  // ✅ allocated in DLL heap
    
    

    Entity& player = s->CreateEntity(50.0f, 100.0f, 30, 80, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    player.addComponent<KeyboardMovementComponent>();
    auto& kms = s->addSystem<KeyboardMovementSystem>(ctx);
    auto& ts = s->addSystem<TransformSystem>();
    Entity& ball = s->CreateEntity(100.0f, 100.0f, 30, 30, glm::vec4(1.0f, 0.5f, 0.3f, 1.0f));
    Ray ballRay = Ray(ball.getComponent<TransformComponent>().pos);
    ballRay.angle = 60.0f;
    ballRay.findScreenEndPoint();
    Entity& player2 = s->CreateEntity(200.0f, 100.0f, 30, 80, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    
    player.addComponent<ColliderComponent>("AABB");
    ball.addComponent<ColliderComponent>("AABB");
    player2.addComponent<ColliderComponent>("AABB");

    auto& cs = s->addSystem<ColliderSystem>();
    // cs.SetAABBSideEffect([s](std::unique_ptr<Entity>& entity, std::unique_ptr<Entity>& other){
    //     other->quad.changeRGBAColor(0.0f, 1.0f, 0.0f, 1.0f, s);
    // });

    Entity* s1 = nullptr;
    Entity* s2 = nullptr;

    cs.SetCustomLogic([&ballRay, s, s1, s2, &player, &player2, &ball](){
        if(ColliderSystem::AABB(player, ball)){
            auto& playerTransform = player.getComponent<TransformComponent>();
            float playerHeight = player.quad.height;
            float playerCenterY = playerTransform.pos.y + (playerHeight / 2.0f);

            ballRay.drawPoints(s, s1, s2);
            // if(ballTransform.pos.y < playerCenterY)
            // {
            //     ballTransform.velocity = StarVec2D(-2.0f, -2.0f); // top half → go up-right
            // }
            // else
            // {
            //     ballTransform.velocity = StarVec2D(2.0f, 2.0f);  // bottom half → go down-right
            // }

        }
    });

    return s;
}