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
#include <chrono>
#include <thread>
#include "StarLine.h"

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

    static void initGLAD(GLADloadproc loaderFunc);

    // Syncs *this DLL's* private ImGui global context pointer (GImGui) to the
    // shared context created by the editor. Must be compiled once into
    // starisk_core.dll (declared here, defined in Starisk.cpp) -- if this were
    // inline in the header, every binary that includes Starisk.h would get its
    // own copy and calling it wouldn't actually set core's copy. Same root
    // cause as the GLAD issue, just for ImGui's global state instead.
    static void initImGuiContext(ImGuiContext* ctx);

    
    StarQuad CreateQuad(float x_pos, float y_pos, float width, float height, const char* texturePath);
    StarQuad CreateQuad(float x_pos, float y_pos, float width, float height, glm::vec4 rgba);
    StarLine CreateLine(StarVec2D p1, StarVec2D p2);

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