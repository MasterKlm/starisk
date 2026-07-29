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



void StarQuad::updateTexture(const char* newTexturePath)
{
    AtlasRegion region = sbm->textureManager.GetOrLoadRegion(newTexturePath, (int)width, (int)height);

    std::vector<float> newVertices = calcStarQuadVertUVFromfWidth(x, y, width, height, region.uMin, region.uMax, region.vMin, region.vMax);

    sbm->move(newVertices, sbm_data);

    vertices = newVertices;
}