#ifndef STARTEXTURE_H
#define STARTEXTURE_H
#include "glad/glad.h"
#include "StarTextureManager.h"
#include "stb_image.h"          
#include "stb_image_resize2.h"
#include "StariskExport.h"


class STARISK_API StarTexture
{

public:
    int width, height, nChannels;
    StarTexture(const char* filePath);
    
    ~StarTexture();
    
    float uMin, uMax, vMin, vMax;

};




#endif