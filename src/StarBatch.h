#ifndef STARBATCH_H
#define STARBATCH_H
#include "Starisk.h"
#include "glad/glad.h"
#include <vector>
#include <optional>


struct BatchConfig
{
    unsigned int uRenderType;
    int iPriority;


    BatchConfig(unsigned renderType, int priority) : uRenderType(renderType), iPriority(priority) {};

    bool operator==(const BatchConfig& other) const
    {
        if(uRenderType != other.uRenderType || iPriority != other.iPriority) return false;
        else return true;
    }

    bool operator!=(const BatchConfig& other) const {return !(*this == other);}


};


class StarBatch 
{
public:
    unsigned int uMaxNumVertices;
    unsigned int uNumOfUsedVertices;
    unsigned int VAO, VBO, EBO;

    std::vector<unsigned int> indices;

    int attribOffset = 0;
    int attribIndex = 0;
    

    BatchConfig _config;
    VertexUV _lastVertex;

    StarBatch(const unsigned maxNumVertices);
    ~StarBatch();
    void prepareRender();
    void render();

    bool isEnoughRoom(const unsigned numOfVertices);    
    //bool isEmpty() const;
    //bool isBatchConfig(const StarBatch& other) const;
    
    void add(const std::vector<VertexUV>& inputVertices, const BatchConfig& other);
    void add(const std::vector<VertexUV>& inputVertices);

    void enableAttribPointerf(int numOfVertices);



protected:
private:
    void cleanUp();





};




#endif