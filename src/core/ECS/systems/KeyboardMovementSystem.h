#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H
#include "../systems.h"
#include "../../io/Keyboard.h"
#include "../../io/Mouse.h"
#include "../components/KeyboardMovementComponent.h"
#include "../components/TransformComponent.h"
#include "../../io/Joystick.h"
#include "../ecs.h"
#include "../../StariskExport.h"
#include "imgui/imgui.h"

class STARISK_API KeyboardMovementSystem : public System
{
public:
    KeyboardMovementSystem() {}
    KeyboardMovementSystem(ImGuiContext* ctx)
    {
        ImGui::SetCurrentContext(ctx);
    }

    // Signature MUST match base class exactly
    void update(StarECSManager* sem, StarBatchManager* sbm) override
    {
        // Block game input when an ImGui panel has keyboard focus
        // std::cout << "WantCaptureKeyboard: " << ImGui::GetIO().WantCaptureKeyboard << "\n";
        // std::cout << "[KMS] update called, W=" << Keyboard::key(GLFW_KEY_W) << "\n";
        // if (ImGui::GetIO().WantTextInput) return;
        // std::cout << "[KMS] update() called, entities=" << sem->entities.size() << "\n";
        
        if (ImGui::GetIO().WantTextInput) { std::cout << "[KMS] blocked by WantTextInput\n"; return; }


        for (auto& entity : sem->entities)
        {
            bool hasKMC = entity->hasComponent<KeyboardMovementComponent>();
            // std::cout << "[KMS] entity checked, hasKMC=" << hasKMC << "\n";
            
            if (!hasKMC) continue;

            auto& kmc = entity->getComponent<KeyboardMovementComponent>();
            // std::cout << "[KMS] kmc.active=" << kmc.active << " W=" << Keyboard::key(GLFW_KEY_W) << "\n";


            auto& transform = entity->getComponent<TransformComponent>();

            if (!kmc.active) continue;

            // Keyboard::key() reads from starisk_core's static array,
            // which chained_key_callback already keeps updated
            // if (Keyboard::key(GLFW_KEY_SPACE)) std::cout << "Space pressed" << "\n";
            if (Keyboard::key(GLFW_KEY_W)){
                transform.y -= 1 * transform.speed;
                // std::cout << "[KMS] new transform.y = " << transform.y << "\n"; 
            } 
            if (Keyboard::key(GLFW_KEY_S)) transform.y += 1 * transform.speed;
            if (Keyboard::key(GLFW_KEY_D)) transform.x += 1 * transform.speed;
            if (Keyboard::key(GLFW_KEY_A)) transform.x -= 1 * transform.speed;
        }
    }
};

#endif