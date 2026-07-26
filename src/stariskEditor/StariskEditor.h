#ifndef STARISKEDITOR_H
#define STARISKEDITOR_H
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <string>
#include <stack>
#include "../core/StarUtils.h"
#include <istream>
#include <unordered_map>
#include <fstream>
#include <windows.h>
#include <map>
#include <chrono>
#include <thread>

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
    int gameViewWidth = 1000, gameViewHeight = 600;
    std::stack<std::string> popupMessagesStack;
    std::unordered_map<std::string, GLuint> icons;

    StariskEditor();
    ~StariskEditor();
    
    void createWindow();
    void initGameFramebuffer(int w, int h);
    void closeCurrentProject();
    void displayProject(const char* projectPath);
    void mainLoop();
    void processInput();
    void showPopups();

    void addPopupMessage(std::string msg);

    void selectGameProjectUI();

};


#endif