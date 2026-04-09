#ifndef STARQUAD_H
#define STARQUAD_H
#include "Starisk.h"
#include <vector>
#include "StarTexture.h"
#include <optional>

class StarQuad
{

public:
    float x = 0.0f, y = 0.0f, z = 0.0f;
    float width, height;
    std::optional<StarTexture> texture;

    std::vector<VertexUV> vertices;

    StarQuad(float x_pos, float y_pos, float w, float h, const char* texturePath);
    StarQuad(float x_pos, float y_pos, float w, float h);  // no texture quad
    ~StarQuad();




};




#endif