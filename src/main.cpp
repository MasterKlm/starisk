#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Starisk.h"
#include "glm/glm.hpp"
#include "glm/common.hpp"
#include "ECS/components.h"



int main()
{

 
    Starisk starisk;
    starisk.createWindow(1000, 600, "Starisk **");
    
    /*auto blueBox = starisk.CreateQuad(100.0f, 20.0f, 100.0f, 100.0f, "assets/img/sky_box.png");
    auto blueBox2 = starisk.CreateQuad(600.0f, 20.0f, 100.0f, 100.0f, "assets/img/sky_box.png");
    auto blueBox3 = starisk.CreateQuad(40.0f, 200.0f, 50.0f, 100.0f, "assets/img/sky_box.png");
    auto blueBox4 = starisk.CreateQuad(100.0f, 180.0f, 120.0f, 60.0f, "assets/img/sky_box.png");
    auto blueBox5 = starisk.CreateQuad(290.0f, 20.0f, 120.0f, 50.0f, "assets/img/sky_box.png");
    auto redBox = starisk.CreateQuad(330.0f, 200.0f, 200.0f, 100.0f, glm::vec4(1.0f,0.0f,0.0f,1.0f));*/

    Entity& player = starisk.CreateEntity(100.0f, 100.0f, 30, 70, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    
    player.addComponent<KeyboardMovementComponent>();

    starisk.mainLoop();

        

    glfwTerminate();
    return 0;
}

