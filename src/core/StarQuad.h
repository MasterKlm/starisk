#ifndef STARQUAD_H
#define STARQUAD_H

#include <vector>
#include "StarTexture.h"
#include "StarBatchManager.h"
#include "StarUtils.h"
#include <optional>
#include "StarVertex.h"
#include "StariskExport.h"

class STARISK_API Starisk; 

//template <typename T>
class STARISK_API StarQuad
{

public:
    float x = 0.0f, y = 0.0f, z = 0.0f;
    float width, height;
    SBM_DATA sbm_data;
    std::optional<StarTexture> texture;
    std:: optional<glm::vec4> rgba;
    //T vertexType;

    std::vector<float> vertices;

    StarQuad(){};

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
    :x(x_pos), y(y_pos), width(w), height(h)
    {
        //vertices = calcStarQuadVertFromfWidth(x_pos, y_pos, w, h);
    }  // no texture quad
    
    StarQuad(float x_pos, float y_pos, float w, float h, glm::vec4 color_rgba)
    :x(x_pos), y(y_pos), width(w), height(h)
    {
        vertices = calcStarQuadVertRGBAFromfWidth(x_pos, y_pos, w, h, color_rgba);
        rgba = color_rgba;
        
    }

    ~StarQuad(){};
    
    void changeRGBAColor(float r, float g, float b, float a, Starisk& starisk);
    


    void move(float x_pos, float y_pos, StarBatchManager* sbm)
    {
        if(x == x_pos && y == y_pos) return;
        std::vector<float> newVertices;
        if(texture.has_value())
        {
            newVertices = calcStarQuadVertUVFromfWidth(x_pos, y_pos, width, height, texture->uMin, texture->uMax, texture->vMin, texture->vMax);
            
        }
        if(rgba.has_value())
        {
            newVertices = calcStarQuadVertRGBAFromfWidth(x_pos, y_pos, width, height, rgba.value());

        }

        vertices = newVertices;
        std::cout << "[TS] moving quad to x=" << x_pos << " y=" << y_pos << "\n";

        

        sbm->move(newVertices, sbm_data);
    }




};




#endif