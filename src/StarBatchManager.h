#ifndef STARBATCHMANAGER_H
#define STARBATCHMANAGER_H
#include <vector>
#include "StarBatch.h"
#include "StarVertex.h"
#include "StarTextureManager.h"
#include "shader.h"


class StarBatchManager
{

public:
    StarBatchManager();
    ~StarBatchManager();

    StarTextureManager textureManager{};
    Shader uvShader = Shader("assets/vertex_core.shader", "assets/fragment_core.shader");
    Shader colorShader = Shader("assets/vertex_flat_color.shader", "assets/fragment_flat_color.shader");
    
    int prevTotalBatchMemory_VertexUV = 0;
    int prevTotalBatchMemory_VertexRGBA = 0;
    
    void add(const std::vector<float> &v);
    void allocate(const std::vector<VertexUV> &v) noexcept;
    void allocate(const std::vector<VertexRGBA> &v) noexcept;
    void createNewBatchesIfNeeded();
    void update();


private:
    std::vector<StarBatch<VertexUV>> uvBatch;
    std::vector<StarBatch<VertexRGBA>> colorBatch;



};



#endif