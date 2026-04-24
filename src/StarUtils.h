#ifndef STARUTILS_H
#define STARUTILS_H
#include "StarSettings.h"
#include <glad/glad.h>   
#include <GLFW/glfw3.h> 
#include "glm/glm.hpp"
#include "glm/common.hpp"



inline void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0, width, height);
}

inline std::vector<float> calcStarQuadVertUVFromfWidth(float x, float y, float w, float h, float uMin, float uMax, float vMin, float vMax)
{
    //float scaler = 10.0f;
    //Pixel to Ndc conversion
    float nx, ny, nw, nh;
    nx = (x / StariskSettings::WINDOW_WIDTH)    * 2.0f - 1.0f;
    ny = 1.0f - (y / StariskSettings::WINDOW_HEIGHT) * 2.0f;
    nw = (w / StariskSettings::WINDOW_WIDTH)    * 2.0f;
    nh = (h / StariskSettings::WINDOW_HEIGHT)   * 2.0f;
    float z = 0.0f;
    

    std::vector<float> vertices = {
        (nx + nw),     ny,         z,  uMax, vMin,  // top-right
        nx,            ny,         z,  uMin, vMin,  // top-left
        nx,            (ny - nh),  z,  uMin, vMax,  // bottom-left
        (nx + nw),     (ny - nh),  z,  uMax, vMax   // bottom-right
    };

    return vertices;
}


inline std::vector<float> calcStarQuadVertRGBAFromfWidth(float x, float y, float w, float h, const glm::vec4& rgba)
{
    //float scaler = 10.0f;
    //Pixel to Ndc conversion
    float nx, ny, nw, nh;
    nx = (x / StariskSettings::WINDOW_WIDTH)    * 2.0f - 1.0f;
    ny = 1.0f - (y / StariskSettings::WINDOW_HEIGHT) * 2.0f;
    nw = (w / StariskSettings::WINDOW_WIDTH)    * 2.0f;
    nh = (h / StariskSettings::WINDOW_HEIGHT)   * 2.0f;
    float z = 0.0f;
    

    std::vector<float> vertices = {
        (nx + nw),     ny,         z, rgba.r, rgba.g, rgba.b, rgba.a,  // top-right
        nx,            ny,         z, rgba.r, rgba.g, rgba.b, rgba.a,  // top-left
        nx,            (ny - nh),  z, rgba.r, rgba.g, rgba.b, rgba.a,  // bottom-left
        (nx + nw),     (ny - nh),  z, rgba.r, rgba.g, rgba.b, rgba.a   // bottom-right
    };

    return vertices;
}

inline std::vector<float> calcStarQuadVertFromfWidth(float x, float y, float w, float h)
{
    //float scaler = 10.0f;
    //Pixel to Ndc conversion
    float nx, ny, nw, nh;
    nx = (x / StariskSettings::WINDOW_WIDTH)    * 2.0f - 1.0f;
    ny = 1.0f - (y / StariskSettings::WINDOW_HEIGHT) * 2.0f;
    nw = (w / StariskSettings::WINDOW_WIDTH)    * 2.0f;
    nh = (h / StariskSettings::WINDOW_HEIGHT)   * 2.0f;
    float z = 0.0f;
    

    std::vector<float> vertices = {
        (nx + nw),     ny,         z,  // top-right
        nx,            ny,         z,  // top-left
        nx,            (ny - nh),  z,  // bottom-left
        (nx + nw),     (ny - nh),  z   // bottom-right
    };

    return vertices;
}

#endif