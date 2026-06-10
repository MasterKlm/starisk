#ifndef STARISKEDITOR_H
#define STARISKEDITOR_H
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <string>
#include <istream>
#include <fstream>
#include <windows.h>


class StariskEditor
{

public:
    int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
    GLFWwindow* window = nullptr;
    ImGuiContext* ctx = nullptr;
    ImGuiIO* io = nullptr;
    HMODULE projectDLL = nullptr;
    GLuint gameFramebuffer = 0;
    GLuint gameColorTexture = 0;
    int gameViewWidth = 800, gameViewHeight = 600;

    // StariskEditor();
    ~StariskEditor();
    
    void createWindow();
    void initGameFramebuffer(int w, int h);
    void displayProject(const char* projectPath);
    void mainLoop();
    void processInput();

};


#endif