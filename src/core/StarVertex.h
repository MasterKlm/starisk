#ifndef STARVERTEX_H
#define STARVERTEX_H
#include "glm/glm.hpp"
#include "StariskExport.h"

struct STARISK_API SBM_DATA     { int startBufferIndex; int starBatchIndex;};
struct STARISK_API VertexUV     { float x, y, z, u, v;};
struct STARISK_API VertexRGBA   { float x, y, z; glm::vec4 rgba;};
struct STARISK_API Vertex       { float x, y, z; };

#endif