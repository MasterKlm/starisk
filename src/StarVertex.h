#ifndef STARVERTEX_H
#define STARVERTEX_H
#include "glm/glm.hpp"


struct SBM_DATA     { int startBufferIndex; int starBatchIndex;};
struct VertexUV     { float x, y, z, u, v;};
struct VertexRGBA   { float x, y, z; glm::vec4 rgba;};
struct Vertex       { float x, y, z; };

#endif