#include "shader.h"


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{   

    int success;
    char infoLog[512];

    GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    //catch errors
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "Error Linking shader: " << "\n" << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::activate()
{
    glUseProgram(id);
}

std::string Shader::loadShaderSrc(const char* filePath)
{
    std::ifstream file;
    std::stringstream buf;
    std::string ret = "";

    file.open(filePath);

    if(file.is_open())
    {
        buf << file.rdbuf();
        ret = buf.str();
    }
    else
    {
        std::cout << "Could not open file path: " << filePath << "\n";
    }

    file.close();
    return ret;
}


GLuint Shader::compileShader(const char* filePath, GLenum type){

    int success;
    char infoLog[512];

    GLuint ret = glCreateShader(type);
    std::string shaderSrc = loadShaderSrc(filePath);
    const GLchar* shader = shaderSrc.c_str();

    glShaderSource(ret, 1, &shader, NULL);
    glCompileShader(ret);

    //catch errors
    glGetShaderiv(ret, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(ret, 512, NULL, infoLog);
        std::cout << "Error compiling shader: " << "\n" << infoLog << "\n";
    }

    return ret;
}


void Shader::setInt(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(id, name), value);
}

