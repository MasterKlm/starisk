#ifndef STARISK_H
#define STARISK_H
#include <iostream>
#include "glm/glm.hpp"
#include "glm/common.hpp"

struct VertexUV {
    float x, y, z;
    float u, v;
};

struct VertexRGBA {
    float x, y, z;
    glm::vec4 rgba;
};

struct Vertex {
    float x, y, z;
};


class Starisk
{
public:
    static int WINDOW_WIDTH, WINDOW_HEIGHT;


    Starisk();
    ~Starisk();

    void init();
};

#endif