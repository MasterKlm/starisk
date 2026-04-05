#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"
#include "stb_image_resize2.h"
#include <vector>
#include "TextureManager.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


struct Vertex {
    float x, y, z;
    float u, v;
};





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

    GLFWwindow* window = glfwCreateWindow(800,600, "OpenGl", NULL, NULL);

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

    TextureManager textureManager{};
    
    Texture blueBox("assets/img/sky_box.png");


    std::cout << "UVs: " << blueBox.uMin << ", " << blueBox.vMin << ", " << blueBox.uMax << ", " << blueBox.vMax << "\n";

    Vertex vertices[] = {
        { 0.5f,  0.5f, 0.0f,     blueBox.uMin, blueBox.vMin},
        {-0.5f,  0.5f, 0.0f,     blueBox.uMax, blueBox.vMin},
        {-0.5f, -0.5f, 0.0f,     blueBox.uMax, blueBox.vMax},
        { 0.5f, -0.5f, 0.0f,     blueBox.uMin, blueBox.vMax}
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    int stride = sizeof(Vertex);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    
    shader.activate();
    shader.setInt("u_Atlas", 0);
    //shader.setHandleui64ARB("ourTexture", handle);


    while(!glfwWindowShouldClose(window))
        {
            //proccess input
            processInput(window);

            //render
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            textureManager.Bind();
            shader.activate();
            glBindVertexArray(VAO);

            

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

