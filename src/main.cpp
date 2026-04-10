#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"
#include "stb_image_resize2.h"
#include <vector>
#include "StarTextureManager.h"
#include "StarTexture.h"
#include "Starisk.h"
#include "StarQuad.h"
#include "StarBatch.h"
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

    GLFWwindow* window = glfwCreateWindow(Starisk::WINDOW_WIDTH, Starisk::WINDOW_HEIGHT, "OpenGl", NULL, NULL);

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

    Shader shader("assets/vertex_core.shader", "assets/fragment_core.shader");
    Shader flatColorShader("assets/vertex_flat_color.shader", "assets/fragment_flat_color.shader");

    StarTextureManager textureManager{};
    
    StarQuad<VertexUV> blueBox(100.0f, 20.0f, 100.0f, 100.0f, "assets/img/sky_box.png");
    StarQuad<VertexUV> blueBox2(600.0f, 20.0f, 100.0f, 100.0f, "assets/img/sky_box.png");
    StarQuad<VertexUV> blueBox3(400.0f, 200.0f, 50.0f, 100.0f, "assets/img/sky_box.png");
    StarQuad<VertexUV> blueBox4(600.0f, 480.0f, 120.0f, 60.0f, "assets/img/sky_box.png");
    StarQuad<VertexUV> blueBox5(290.0f, 20.0f, 40.0f, 80.0f, "assets/img/sky_box.png");
    StarQuad<VertexRGBA> redBox(330.0f, 200.0f, 200.0f, 100.0f, glm::vec4(1.0f,0.0f,0.0f,1.0f));


    StarBatch<VertexUV> batch(1000);
    StarBatch<VertexRGBA> colorBatch(1000);

    batch.add(blueBox.vertices);
    batch.add(blueBox2.vertices);
    batch.add(blueBox3.vertices);
    batch.add(blueBox4.vertices);
    batch.add(blueBox5.vertices);
    
    colorBatch.add(redBox.vertices);
    
    shader.activate();
    shader.setInt("u_Atlas", 0);
    flatColorShader.activate();
    //shader.setHandleui64ARB("ourTexture", handle);

    batch.prepareRender();
    colorBatch.prepareRender();

    while(!glfwWindowShouldClose(window))
        {
            //proccess input
            processInput(window);

            //render
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            textureManager.Bind();
            shader.activate();
            batch.render();
            flatColorShader.activate();
            colorBatch.render();
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

