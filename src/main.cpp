#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "stb_image_resize2.h"
#include <vector>
#include "Starisk.h"
#include "StarSettings.h"
#include "glm/glm.hpp"
#include "glm/common.hpp"




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);





int main()
{

    int success;
    char infoLog[512];

    glfwInit();

    //version 3.4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //apple only
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(StariskSettings::WINDOW_WIDTH, StariskSettings::WINDOW_HEIGHT, "Starisk **", NULL, NULL);

    if(window == NULL){
        std::cout << "Window creaion failed" << "\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize glad" << '\n';
        glfwTerminate();
        return -1;
    }

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "GPU: " << renderer << "\n";
    std::cout << "OpenGL version: " << version << "\n";

    //set view port
    
    glViewport(0,0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Starisk starisk;
    
    auto blueBox = starisk.CreateQuad(100.0f, 20.0f, 100.0f, 100.0f, "assets/img/sky_box.png");
    auto blueBox2 = starisk.CreateQuad(600.0f, 20.0f, 100.0f, 100.0f, "assets/img/sky_box.png");
    auto blueBox3 = starisk.CreateQuad(40.0f, 200.0f, 50.0f, 100.0f, "assets/img/sky_box.png");
    auto blueBox4 = starisk.CreateQuad(100.0f, 180.0f, 120.0f, 60.0f, "assets/img/sky_box.png");
    auto blueBox5 = starisk.CreateQuad(290.0f, 20.0f, 120.0f, 50.0f, "assets/img/sky_box.png");
    auto redBox = starisk.CreateQuad(330.0f, 200.0f, 200.0f, 100.0f, glm::vec4(1.0f,0.0f,0.0f,1.0f));

    while(!glfwWindowShouldClose(window))
        {
            //proccess input
            processInput(window);

            //render
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            starisk.mainLoop([](){
                
            });
            //send new frame to, and generate kind of before hand and prevent flicketing
            glfwSwapBuffers(window);
            glfwPollEvents();


        }

        

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

