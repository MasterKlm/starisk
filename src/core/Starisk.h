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
#include "ECS/systems.h"
#include "StariskExport.h"
#include "imgui/imgui.h"

class STARISK_API Starisk
{
public:

    StarBatchManager* sbm = nullptr;
    StarECSManager sem{};
    GLFWwindow* window = nullptr;

    

    Starisk();
    Starisk(GLFWwindow* existingWindow);
    ~Starisk();

    template <typename T, typename... Targs>
    T& addSystem(Targs&&... mArgs)
    {
        return sem.addSystem<T>(std::forward<Targs>(mArgs)...);
    }

    static void initGLAD(GLADloadproc loaderFunc)
    {
        if (!gladLoadGLLoader(loaderFunc)) {
            std::cout << "[Starisk] GLAD re-init failed\n";
        } else {
            std::cout << "[Starisk] GLAD re-init success\n";
        }
    }
        
    
    StarQuad CreateQuad(float x_pos, float y_pos, float width, float height, const char* texturePath);
    StarQuad CreateQuad(float x_pos, float y_pos, float width, float height, glm::vec4 rgba);

    Entity& CreateEntity(float xpos, float ypos, int width, int height, const char* texturePath);
    Entity& CreateEntity(float xpos, float ypos, int width, int height); //default texture placeholder
    Entity& CreateEntity(float xpos, float ypos, int width, int height, glm::vec4 rgba);
    
    void mainLoop(std::function<void()> fn, bool isEditor = false);
    void mainLoop(bool isEditor = false);
    void init();
    void initStariskBaseSystems();
    void injectKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

    void setGameRunning(bool v);
    bool isGameRunning();
    
    void createWindow(int width, int height, const char* title);
    void processInput();
    
private:
    bool gameRunning = true;
};

#endif