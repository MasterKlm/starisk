#ifndef STARQUAD_H
#define STARQUAD_H
#include "Starisk.h"
#include <vector>
#include "StarTexture.h"
#include "StarUtils.h"
#include "glm/glm.hpp"
#include "glm/common.hpp"
#include <optional>

template <typename T>
class StarQuad
{

public:
    float x = 0.0f, y = 0.0f, z = 0.0f;
    float width, height;
    std::optional<StarTexture> texture;

    std::vector<T> vertices;

    StarQuad(float x_pos, float y_pos, float w, float h, const char* texturePath): x(x_pos), y(y_pos), width(w), height(h)
    {
        texture = StarTexture(texturePath);
        if(texture.has_value())
        {
            vertices = calcStarQuadVertUVFromfWidth(x_pos, y_pos, w, h, texture->uMin, texture->uMax, texture->vMin, texture->vMax);
        }
        else
        {
            std::cout << "Tetxure not loaded" << "\n";
        }
    }


    StarQuad(float x_pos, float y_pos, float w, float h)
    : x(x_pos), y(y_pos), width(w), height(h)
    {
        //vertices = calcStarQuadVertFromfWidth(x_pos, y_pos, w, h);
    }  // no texture quad
    
    StarQuad(float x_pos, float y_pos, float w, float h, glm::vec4 rgba)
    : x(x_pos), y(y_pos), width(w), height(h)
    {
        vertices = calcStarQuadVertRGBAFromfWidth(x_pos, y_pos, w, h, rgba);
        
    }

    ~StarQuad(){};




};




#endif