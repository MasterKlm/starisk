#ifndef STARISKEDITOR_H
#define STARISKEDITOR_H
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"


class StariskEditor
{

public:
    int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
    GLFWwindow* window = nullptr;
    ImGuiIO* io = nullptr;


    // StariskEditor();

    void createWindow();
    void mainLoop();
    void processInput();

};


#endif