#include "../../core/Starisk.h"
#include <iostream>


StariskMain mainProject(GLFWwindow* window, ImGuiContext* ctx)
{

    Starisk* starisk = new Starisk(window, ctx);

    Entity& player = starisk->CreateEntity(50.0f, 100.0f, 30, 80);
    player.addComponent<KeyboardMovementComponent>();
    
    auto& kms = starisk->addSystem<KeyboardMovementSystem>(ctx);




    return starisk;

}