#ifndef STARBATCHMANAGER_H
#define STARBATCHMANAGER_H
#include <vector>
#include "StarBatch.h"
#include "StarVertex.h"
#include "StarTextureManager.h"
#include "shader.h"
#include "StariskExport.h"

class STARISK_API StarBatchManager
{

public:
    StarBatchManager();
    ~StarBatchManager();

    StarBatchManager(StarBatchManager&&) noexcept = default;
    StarBatchManager& operator=(StarBatchManager&&) noexcept = default;

    StarTextureManager textureManager{};
    Shader uvShader = Shader("assets/vertex_core.shader", "assets/fragment_core.shader");
    Shader colorShader = Shader("assets/vertex_flat_color.shader", "assets/fragment_flat_color.shader");
    
    int prevTotalBatchMemory_VertexUV = 0;
    int prevTotalBatchMemory_VertexRGBA = 0;
    
    SBM_DATA add(const std::vector<float> &v);
    SBM_DATA allocate(const std::vector<VertexUV> &v) noexcept;
    SBM_DATA allocate(const std::vector<VertexRGBA> &v) noexcept;
    void move(std::vector<float> newVertices, SBM_DATA sbm_data);
    void createNewBatchesIfNeeded();
    void update();
    int getGroudBatchId();

    
    int getBatchId();

private:
    std::vector<StarBatch<VertexUV>> uvBatch;
    std::vector<StarBatch<VertexRGBA>> colorBatch;



};



#endif