#include "StarQuad.h"
#include "Starisk.h"

void StarQuad::changeRGBAColor(float r, float g, float b, float a, Starisk& starisk)
{
    if(rgba.has_value())
    {
        glm::vec4 newRGBA(r, g, b, a);
        if(rgba == newRGBA) return;

        rgba = newRGBA;
        std::vector<float> newVertices = calcStarQuadVertRGBAFromfWidth(x, y, width, height, rgba.value());
        vertices = newVertices;

        starisk.sbm->move(newVertices, sbm_data);
    }
}



void StarQuad::changeRGBAColor(float r, float g, float b, float a, Starisk* starisk)
{
    if(rgba.has_value())
    {
        glm::vec4 newRGBA(r, g, b, a);
        if(rgba == newRGBA) return;

        rgba = newRGBA;
        std::vector<float> newVertices = calcStarQuadVertRGBAFromfWidth(x, y, width, height, rgba.value());
        vertices = newVertices;

        starisk->sbm->move(newVertices, sbm_data);
    }
}