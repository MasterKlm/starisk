#ifndef STARISK_H
#define STARISK_H
#include <iostream>

struct VertexUV {
    float x, y, z;
    float u, v;
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