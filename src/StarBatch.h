#ifndef STARBATCH_H
#define STARBATCH_H
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

template <typename T>
class StarBatch 
{
public:
    unsigned int uMaxNumVertices;
    unsigned int uNumOfUsedVertices;
    unsigned int VAO, VBO, EBO;

    std::vector<unsigned int> prevIndices;
    std::vector<unsigned int> indices;

    int attribOffset = 0;
    int attribIndex = 0;
    

    BatchConfig _config;
    T _lastVertex;

    
    
    StarBatch(const unsigned maxNumVertices)
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * uMaxNumVertices, nullptr, GL_STATIC_DRAW);

        enableAttribPointerf(3);
        enableAttribPointerf(2);
        
    }

    ~StarBatch(){ 
        cleanUp();
        std::cout << "Cleaned up batch" << "\n";
     }


    StarBatch(StarBatch&& other) noexcept
    : uMaxNumVertices(other.uMaxNumVertices),
      uNumOfUsedVertices(other.uNumOfUsedVertices),
      VAO(other.VAO),
      VBO(other.VBO),
      EBO(other.EBO),
      prevIndices(std::move(other.prevIndices)),
      indices(std::move(other.indices)),
      attribOffset(other.attribOffset),
      attribIndex(other.attribIndex),
      _config(other._config),
      _lastVertex(other._lastVertex)
    {
        // Null out the moved-from object's GPU handles
        // so its destructor doesn't delete them
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }

    StarBatch(const StarBatch&) = delete;
    StarBatch& operator=(const StarBatch&) = delete;

    
    void prepareRender()
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



        if(prevIndices != indices)
        {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

            //std::cout << "Batch Memory Usage: " << uNumOfUsedVertices * sizeof(T) << " Bytes" << "\n";

            prevIndices = indices;
        }
    }

    void render()
    {
        if(uNumOfUsedVertices == 0) return;
        if(indices.empty()){
            std::cout << "Run StarBatch.prepareRender() to set indices before calling render()" << "\n";
            return;
        };

        


        glBindVertexArray(VAO);
        glDrawElements(_config.uRenderType, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    }

    bool isEnoughRoom(const unsigned numOfVertices)
    {
        return (uNumOfUsedVertices + numOfVertices <= uMaxNumVertices);
    }    
    //bool isEmpty() const;
    //bool isBatchConfig(const StarBatch& other) const;
    
    void add(const std::vector<T>& inputVertices, const BatchConfig& other){ _config = other; add(inputVertices);}
    
    
    void add(const std::vector<T>& inputVertices)
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
        glBufferSubData(GL_ARRAY_BUFFER, uNumOfUsedVertices * sizeof(T), inputVertices.size() * sizeof(T), &inputVertices[0]);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        uNumOfUsedVertices += inputVertices.size();
        _lastVertex = inputVertices[inputVertices.size() - 1];

    }
    
    void enableAttribPointerf(int numOfVertices){
        glVertexAttribPointer(attribIndex, numOfVertices, GL_FLOAT, GL_FALSE, sizeof(T), (void*)(attribOffset * sizeof(float)));
        glEnableVertexAttribArray(attribIndex);
        attribIndex++;
        attribOffset += numOfVertices;
    }



protected:
private:
    void cleanUp(){
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





};




#endif