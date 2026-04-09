#include "StarQuad.h"
#include "StarUtils.h"


StarQuad::StarQuad(float x_pos, float y_pos, float w, float h, const char* texturePath) 
: x(x_pos), y(y_pos), width(w), height(h)
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

StarQuad::StarQuad(float x_pos, float y_pos, float w, float h) 
: x(x_pos), y(y_pos), width(w), height(h)
{
    //vertices = calcStarQuadVertFromfWidth(x_pos, y_pos, w, h);
}



StarQuad::~StarQuad(){}