#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "StariskEditor.h"



int main()
{

    StariskEditor editor;
    editor.createWindow();
    editor.mainLoop();
        

    glfwTerminate();
    return 0;
}

