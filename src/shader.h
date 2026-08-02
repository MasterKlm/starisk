#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "StariskExport.h"

class STARISK_API Shader
{
public:
    unsigned int id;

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
    void activate();


    std::string loadShaderSrc(const char* filePath);
    GLuint compileShader(const char* filePath, GLenum type);
    void setInt(const char* name, int value);
    
    
};

#endif