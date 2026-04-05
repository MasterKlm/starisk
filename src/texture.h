#ifndef TEXTURE_H
#define TEXTURE_H
#include "glad/glad.h"
#include "TextureManager.h"
#include "stb_image.h"          
#include "stb_image_resize2.h"



class Texture
{

public:
    int width, height, nChannels;
    Texture(const char* filePath);
    
    ~Texture();
    
    float uMin, uMax, vMin, vMax;

};




#endif