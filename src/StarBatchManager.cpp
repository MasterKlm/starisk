#include "StarBatchManager.h"
#include "glad/glad.h"



StarBatchManager::StarBatchManager()
{
    uvBatch.emplace_back(StarBatch<VertexUV>(1000, 0, GL_TRIANGLES));
    colorBatch.emplace_back(StarBatch<VertexRGBA>(1000, 0, GL_TRIANGLES));
    lineBatch.emplace_back(StarBatch<LineVertex>(1000, 0 , GL_LINES));
    //set atlas texture
    uvShader.activate();
    uvShader.setInt("u_Atlas", 0);
}

StarBatchManager::~StarBatchManager()
{

}

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

void StarBatchManager::move(std::vector<LineVertex> lines, SBM_DATA sbm_data)
{
    lineBatch[sbm_data.starBatchIndex].updateVertices(sbm_data.startBufferIndex, lines);
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
    auto& lastLineBatch = lineBatch.back();
    
    if(lastUvBatch.uNumOfUsedVertices == lastUvBatch.uMaxNumVertices)
    {
        // Use the current size as the next index
        int id = (int)uvBatch.size();
        uvBatch.emplace_back(StarBatch<VertexUV>(1000, id));
    }
    
    if(lastColorBatch.uNumOfUsedVertices == lastColorBatch.uMaxNumVertices)
    {
        int id = (int)colorBatch.size();
        colorBatch.emplace_back(StarBatch<VertexRGBA>(1000, id));
    }

    if(lastLineBatch.uNumOfUsedVertices == lastLineBatch.uMaxNumVertices){
        int id = (int)lineBatch.size();
        lineBatch.emplace_back(StarBatch<LineVertex>(1000, id, GL_LINES));
    }

    //std::cout << "Total number of UV batches: " << (int)uvBatch.size() << "\n";
    //std::cout << "Total number of RGBA batches: " << (int)colorBatch.size() << "\n";
}


SBM_DATA StarBatchManager::drawLine(StarVec2D p1, StarVec2D p2, int window_width, int window_height)
{
    float nx1 = (p1.x / window_width) * 2.0f - 1.0f;
    float ny1 = 1.0f - (p1.y / window_height) * 2.0f;
    float nx2 = (p2.x / window_width) * 2.0f - 1.0f;
    float ny2 = 1.0f - (p2.y / window_height) * 2.0f;

    std::vector<LineVertex> verts = { {nx1, ny1}, {nx2, ny2} };
    SBM_DATA data = lineBatch.back().add(verts);
    return data;
}

void StarBatchManager::update()
{
    createNewBatchesIfNeeded();


    int totalBatchMemory_VertexUV  = 0;
    int totalBatchMemory_VertexRGBA  = 0;
    int totalBatchMemory_LineVertex  = 0;

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

    for(auto& lb : lineBatch)
    {
        lb.prepareRender();
        lineShader.activate();
        lb.render();
        totalBatchMemory_LineVertex += lb.uNumOfUsedVertices;
    }

    if(totalBatchMemory_VertexUV != prevTotalBatchMemory_VertexUV) std::cout << "Total Vertex UV Batches GPU Memory Usage: " << totalBatchMemory_VertexUV * sizeof(VertexUV) << " Bytes" << "\n";
    if(totalBatchMemory_VertexRGBA != prevTotalBatchMemory_VertexRGBA) std::cout << "Total Vertex RGBA Batches GPU Memory Usage: " << totalBatchMemory_VertexRGBA * sizeof(VertexRGBA) << " Bytes" << "\n";
    if(totalBatchMemory_LineVertex != prevTotalBatchMemory_LineVertex) std::cout << "Total LineVertex Batches GPU Memory Usage: " << totalBatchMemory_LineVertex * sizeof(LineVertex) << " Bytes" << "\n";

    prevTotalBatchMemory_VertexUV = totalBatchMemory_VertexUV;
    prevTotalBatchMemory_VertexRGBA = totalBatchMemory_VertexRGBA;
    prevTotalBatchMemory_LineVertex = totalBatchMemory_LineVertex;


}