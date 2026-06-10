#include <iostream>
#include "../core/Starisk.h"
#include "../core/ecs/components.h"
#include "imgui/imgui.h"

extern "C" __declspec(dllexport) void runProject(Starisk* s, ImGuiContext* ctx)
{
    if (!s) return;

    ImGui::SetCurrentContext(ctx); // sync the game DLL's ImGui to editor's context

    

    Entity& player = s->CreateEntity(50.0f, 100.0f, 30, 80, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    player.addComponent<KeyboardMovementComponent>();
    auto& kms = s->addSystem<KeyboardMovementSystem>(ctx);
    
    // Entity& ball = s->CreateEntity(100.0f, 100.0f, 30, 30, glm::vec4(1.0f, 0.5f, 0.3f, 1.0f));
    // Entity& player2 = s->CreateEntity(200.0f, 100.0f, 30, 80, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}