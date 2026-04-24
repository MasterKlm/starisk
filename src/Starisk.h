#ifndef STARISK_H
#define STARISK_H
#include <iostream>
#include "glm/glm.hpp"
#include "glm/common.hpp"
#include "StarQuad.h"
#include "StarBatchManager.h"
#include "StarUtils.h"
#include "StarSettings.h"
#include <functional>
#include <GLFW/glfw3.h>
#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Joystick.h"
#include "ECS/ecs.h"
#include "ECS/components/TransformComponent.h"



class Starisk
{
public:

    StarBatchManager* sbm = nullptr;
    StarECSManager sem{};
    GLFWwindow* window = nullptr;
    
    Starisk();
    ~Starisk();
    
    StarQuad CreateQuad(float x_pos, float y_pos, float weight, float height, const char* texturePath);
    StarQuad CreateQuad(float x_pos, float y_pos, float weight, float height, glm::vec4 rgba);

    Entity& CreateEntity(float xpos, float ypos, int width, int height, const char* texturePath);
    Entity& CreateEntity(float xpos, float ypos, int width, int height, glm::vec4 rgba);
    
    void mainLoop(std::function<void()> fn);
    void mainLoop();
    void init();

    void setGameRunning(bool v);
    bool isGameRunning();
    
    void createWindow(int width, int height, const char* title);
    void processInput();
    
private:
    bool gameRunning = true;
};

#endif