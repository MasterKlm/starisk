#include "StarBatchManager.h"



StarBatchManager::StarBatchManager()
{
    int id = getBatchId();
    uvBatch.emplace_back(StarBatch<VertexUV>(1000, id));
    colorBatch.emplace_back(StarBatch<VertexRGBA>(1000, id));

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


int StarBatchManager::getBatchId()
{
    //std::cout << "called getBatchId()" << "\n";/900
    static int lastId = 0;
    return lastId++;
}

// int StarBatchManager::getGroupBatchId()
// {

// }

SBM_DATA StarBatchManager::add(const std::vector<float> &v)
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
        SBM_DATA ret = allocate(uV);
        return ret;     
    }
    else if(v.size() % rgbaFloats == 0)
    {
        std::vector<VertexRGBA> colorV;
        
        for(int i = 0; i < (int)v.size(); i += rgbaFloats)
        {
            
            colorV.emplace_back(VertexRGBA{v[i], v[i + 1], v[i + 2], glm::vec4(v[i +3], v[i + 4], v[i + 5], v[i + 6])});
        }
        //allocate to a relevant batch
        SBM_DATA ret = allocate(colorV);
        return ret;
    }
    else 
    {
        std::cout << "Unrecognized Vertex Structure" << "\n";
        return SBM_DATA{-1, -1}; 
    }

}

SBM_DATA StarBatchManager::allocate(const std::vector<VertexUV> &v) noexcept
{
    
    //int starBatchIndex = (int)uvBatch.size() - 1;
    SBM_DATA data = uvBatch.back().add(v);
    
    return data;
}

void StarBatchManager::move(std::vector<float> newVertices, SBM_DATA sbm_data)
{
    constexpr std::size_t uvQuadFloats   = (sizeof(VertexUV)   / sizeof(float)) * 4;  // 20
    constexpr std::size_t rgbaQuadFloats = (sizeof(VertexRGBA) / sizeof(float)) * 4;  // 28

    if(newVertices.size() == uvQuadFloats)
    {
        uvBatch[sbm_data.starBatchIndex].updateVertices(sbm_data.startBufferIndex, newVertices);
    }
    else if(newVertices.size() == rgbaQuadFloats)
    {
        colorBatch[sbm_data.starBatchIndex].updateVertices(sbm_data.startBufferIndex, newVertices);
    }
}



SBM_DATA StarBatchManager::allocate(const std::vector<VertexRGBA> &v) noexcept
{

    SBM_DATA data = colorBatch.back().add(v);
    
    return data;
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

    auto& lastUvBatch = uvBatch.back();
    auto& lastColorBatch = colorBatch.back();
    
    if(lastUvBatch.uNumOfUsedVertices == lastUvBatch.uMaxNumVertices)
    {
        int id = getBatchId();
        //if((id + 1) >= (int)uvBatch.size()) id--;

        uvBatch.emplace_back(StarBatch<VertexUV>(1000, id));

    }
    
    if(lastColorBatch.uNumOfUsedVertices == lastColorBatch.uMaxNumVertices)
    {
        int id = getBatchId();
        //if((id + 1) >= (int)uvBatch.size()) id--;
        colorBatch.emplace_back(StarBatch<VertexRGBA>(1000, id));
    }

    //std::cout << "Total number of UV batches: " << (int)uvBatch.size() << "\n";
    //std::cout << "Total number of RGBA batches: " << (int)colorBatch.size() << "\n";
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