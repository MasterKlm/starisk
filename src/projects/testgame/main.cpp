#include "../../core/Starisk.h"
#include <iostream>


StariskMain mainProject(GLFWwindow* window, ImGuiContext* ctx)
{

    Starisk* starisk = new Starisk(window, ctx);

    Entity& player = starisk->CreateEntity(50.0f, 100.0f, 30, 80);
    player.addComponent<KeyboardMovementComponent>();
    
    auto& kms = starisk->addSystem<KeyboardMovementSystem>(ctx);

    KeyboardMovementSystem::setKeyboardMovementComponentCustomLogic(player.getComponent<KeyboardMovementComponent>(), [starisk, &player](){
        if(Keyboard::key(GLFW_KEY_W)){
            player.quad.updateTexture(starisk->sbm, "assets/img/folder_icon.png");
        }
    });




    return starisk;

}