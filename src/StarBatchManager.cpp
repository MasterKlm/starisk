#include "StarBatchManager.h"



StarBatchManager::StarBatchManager()
{
    uvBatch.emplace_back(StarBatch<VertexUV>(1000));
    colorBatch.emplace_back(StarBatch<VertexRGBA>(1000));

    //set atlas texture
    uvShader.activate();
    uvShader.setInt("u_Atlas", 0);
}

StarBatchManager::~StarBatchManager()
{
    // for(auto b : uvBatch)
    // {
    //     b.~StarBatch();
    // }

    // for(auto b : colorBatch)
    // {
    //     b.~StarBatch();
    // }
}

void StarBatchManager::add(const std::vector<float> &v)
{
    constexpr int uvFloats      =   sizeof(VertexUV)    / sizeof(float); // 5
    constexpr int rgbaFloats    =   sizeof(VertexRGBA)   / sizeof(float); //7

    if(v.size() % uvFloats == 0)
    {
        std::vector<VertexUV> uV;

        for(int i = 0; i < (int)v.size(); i += uvFloats)
        {
            uV.emplace_back(VertexUV{v[i], v[i + 1], v[i + 2], v[i +3], v[i + 4]});
        }
        //allocate to a relevant batch
        allocate(uV);       
    }
    else if(v.size() % rgbaFloats == 0)
    {
        std::vector<VertexRGBA> colorV;
        
        for(int i = 0; i < (int)v.size(); i += rgbaFloats)
        {
            
            colorV.emplace_back(VertexRGBA{v[i], v[i + 1], v[i + 2], glm::vec4(v[i +3], v[i + 4], v[i + 5], v[i + 6])});
        }
        //allocate to a relevant batch
        allocate(colorV);
    }
    else 
    {
        std::cout << "Unrecognized Vertex Structure" << "\n";
        return;
    }

}

void StarBatchManager::allocate(const std::vector<VertexUV> &v) noexcept
{
    if((int)uvBatch.size() == 0) return;
    uvBatch[uvBatch.size() - 1].add(v);
    createNewBatchesIfNeeded();

}


void StarBatchManager::allocate(const std::vector<VertexRGBA> &v) noexcept
{
    if((int)colorBatch.size() == 0) return;
    colorBatch[colorBatch.size() - 1].add(v);
    createNewBatchesIfNeeded();

}


void StarBatchManager::createNewBatchesIfNeeded()
{
    if((int)uvBatch.size() == 0) {
        std::cout << "Uv batches is empty" << "\n";
        return;
    };
    if((int)colorBatch.size() == 0) {
        std::cout << "Color batches is empty" << "\n";
        return;
    };

    auto& lastUvBatch = uvBatch[uvBatch.size() - 1];
    auto& lastColorBatch = colorBatch[colorBatch.size() - 1];
    
    if(lastUvBatch.uNumOfUsedVertices == lastUvBatch.uMaxNumVertices)
    {
        uvBatch.emplace_back(StarBatch<VertexUV>(1000));
    }
    
    if(lastColorBatch.uNumOfUsedVertices == lastColorBatch.uMaxNumVertices)
    {
        colorBatch.emplace_back(StarBatch<VertexRGBA>(1000));
    }

    std::cout << "Total number of UV batches: " << (int)uvBatch.size() << "\n";
    std::cout << "Total number of RGBA batches: " << (int)colorBatch.size() << "\n";
}
void StarBatchManager::update()
{
    createNewBatchesIfNeeded();


    int totalBatchMemory_VertexUV  = 0;
    int totalBatchMemory_VertexRGBA  = 0;

    textureManager.Bind();


    for(auto& uv : uvBatch)
    {
        uv.prepareRender();
        uvShader.activate();
        uv.render();
        totalBatchMemory_VertexUV += uv.uNumOfUsedVertices;
    }

    for(auto& cb : colorBatch)
    {
        cb.prepareRender();
        colorShader.activate();
        cb.render();
        totalBatchMemory_VertexRGBA += cb.uNumOfUsedVertices;
    }

    if(totalBatchMemory_VertexUV != prevTotalBatchMemory_VertexUV) std::cout << "Total Vertex UV Batches Memory Usage: " << totalBatchMemory_VertexUV * sizeof(VertexUV) << " Bytes" << "\n";
    if(totalBatchMemory_VertexRGBA != prevTotalBatchMemory_VertexRGBA) std::cout << "Total Vertex RGBA Batches Memory Usage: " << totalBatchMemory_VertexRGBA * sizeof(VertexRGBA) << " Bytes" << "\n";

    prevTotalBatchMemory_VertexUV = totalBatchMemory_VertexUV;
    prevTotalBatchMemory_VertexRGBA = totalBatchMemory_VertexRGBA;


}