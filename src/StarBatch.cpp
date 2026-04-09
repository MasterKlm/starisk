#include "StarBatch.h"


StarBatch::StarBatch(const unsigned maxNumVertices)
: uMaxNumVertices(maxNumVertices), uNumOfUsedVertices(0), VAO(0), VBO(0), _config(GL_TRIANGLES, 0)
{
    EBO = 0;

    if(uMaxNumVertices < 1000)
    {
        std::cout << "max vertices of " << uMaxNumVertices << " is too small. Must be atleast 1000" << "\n";
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUV) * uMaxNumVertices, nullptr, GL_STATIC_DRAW);

    enableAttribPointerf(3);
    enableAttribPointerf(2);
    

    


    
}

void StarBatch::prepareRender()
{
    indices.clear();

    for(int i = 0; i < uNumOfUsedVertices; i += 4)
    {
        if(i + 3 < uNumOfUsedVertices)
        {
            // First triangle
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
            
            // Second triangle
            indices.push_back(i + 2);
            indices.push_back(i + 3);
            indices.push_back(i);
        }
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    std::cout << "Batch Memory Usage: " << uNumOfUsedVertices * sizeof(VertexUV) << " Bytes" << "\n";
}
void StarBatch::render()
{
    if(uNumOfUsedVertices == 0) return;
    if(indices.empty()){
        std::cout << "Run StarBatch.prepareRender() to set indices before calling render()" << "\n";
        return;
    };

    


    glBindVertexArray(VAO);
    glDrawElements(_config.uRenderType, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}


void StarBatch::enableAttribPointerf(int num)
{
    glVertexAttribPointer(attribIndex, num, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)(attribOffset * sizeof(float)));
    glEnableVertexAttribArray(attribIndex);
    attribIndex++;
    attribOffset += num;
}

void StarBatch::add(const std::vector<VertexUV>& inputVertices, const BatchConfig& other) { _config = other; add(inputVertices);}

void StarBatch::add(const std::vector<VertexUV>& inputVertices)
{
    if(inputVertices.size() > uMaxNumVertices)
    {
        std::cout << "Input vertices are over the limit of batch size " << uMaxNumVertices << "\n";
        return;
    }

    if(inputVertices.empty())
    {
        std::cout << "Cannot add empty inputVertices" << "\n";
        return;
    }

    if(!isEnoughRoom(inputVertices.size()))
    {
        std::cout << "Not enough room for vertices of size " << inputVertices.size() << "\n";
        return;
    }


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, uNumOfUsedVertices * sizeof(VertexUV), inputVertices.size() * sizeof(VertexUV), &inputVertices[0]);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    uNumOfUsedVertices += inputVertices.size();
    _lastVertex = inputVertices[inputVertices.size() - 1];

}


bool StarBatch::isEnoughRoom(const unsigned numOfVertices)
{
    return (uNumOfUsedVertices + numOfVertices <= uMaxNumVertices);
}


StarBatch::~StarBatch(){ cleanUp(); }


void StarBatch::cleanUp()
{
    if(VBO != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if(VAO != 0)
    {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
}
