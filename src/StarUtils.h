#ifndef STARUTILS_H
#define STARUTILS_H
#include "Starisk.h"
#include "glm/glm.hpp"
#include "glm/common.hpp"

std::vector<VertexUV> calcStarQuadVertUVFromfWidth(float x, float y, float w, float h, float uMin, float uMax, float vMin, float vMax)
{
    //float scaler = 10.0f;
    //Pixel to Ndc conversion
    float nx, ny, nw, nh;
    nx = (x / Starisk::WINDOW_WIDTH)    * 2.0f - 1.0f;
    ny = 1.0f - (y / Starisk::WINDOW_HEIGHT) * 2.0f;
    nw = (w / Starisk::WINDOW_WIDTH)    * 2.0f;
    nh = (h / Starisk::WINDOW_HEIGHT)   * 2.0f;
    float z = 0.0f;
    

    std::vector<VertexUV> vertices = {
        {(nx + nw),     ny,         z,  uMax, vMin},  // top-right
        {nx,            ny,         z,  uMin, vMin},  // top-left
        {nx,            (ny - nh),  z,  uMin, vMax},  // bottom-left
        {(nx + nw),     (ny - nh),  z,  uMax, vMax}   // bottom-right
    };

    return vertices;
}


std::vector<VertexRGBA> calcStarQuadVertRGBAFromfWidth(float x, float y, float w, float h, const glm::vec4& rgba)
{
    //float scaler = 10.0f;
    //Pixel to Ndc conversion
    float nx, ny, nw, nh;
    nx = (x / Starisk::WINDOW_WIDTH)    * 2.0f - 1.0f;
    ny = 1.0f - (y / Starisk::WINDOW_HEIGHT) * 2.0f;
    nw = (w / Starisk::WINDOW_WIDTH)    * 2.0f;
    nh = (h / Starisk::WINDOW_HEIGHT)   * 2.0f;
    float z = 0.0f;
    

    std::vector<VertexRGBA> vertices = {
        {(nx + nw),     ny,         z, rgba},  // top-right
        {nx,            ny,         z, rgba},  // top-left
        {nx,            (ny - nh),  z, rgba},  // bottom-left
        {(nx + nw),     (ny - nh),  z, rgba}   // bottom-right
    };

    return vertices;
}

std::vector<Vertex> calcStarQuadVertFromfWidth(float x, float y, float w, float h)
{
    //float scaler = 10.0f;
    //Pixel to Ndc conversion
    float nx, ny, nw, nh;
    nx = (x / Starisk::WINDOW_WIDTH)    * 2.0f - 1.0f;
    ny = 1.0f - (y / Starisk::WINDOW_HEIGHT) * 2.0f;
    nw = (w / Starisk::WINDOW_WIDTH)    * 2.0f;
    nh = (h / Starisk::WINDOW_HEIGHT)   * 2.0f;
    float z = 0.0f;
    

    std::vector<Vertex> vertices = {
        {(nx + nw),     ny,         z},  // top-right
        {nx,            ny,         z},  // top-left
        {nx,            (ny - nh),  z},  // bottom-left
        {(nx + nw),     (ny - nh),  z}   // bottom-right
    };

    return vertices;
}

#endif