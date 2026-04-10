#include "StarTexture.h"
#include <cstring>


StarTexture::StarTexture(const char* filePath)
{
    stbi_set_flip_vertically_on_load(true);  

    unsigned char* data = stbi_load(filePath, &width, &height, &nChannels, 4);

    
    //std::cout << "Image size " << "(" << filePath  << "): " << width << "x" << height << "\n";
    
    //resize image to fit on texture atlas
    unsigned char* resized = (unsigned char*)malloc(StarTextureManager::targetResizeW * StarTextureManager::targetResizeH * 4); 
    AtlasRegion imgReg;
    
    if(data)
    {
        
        stbir_resize_uint8_linear(
            data, width, height, 0,
            resized, StarTextureManager::targetResizeW, StarTextureManager::targetResizeH, 0,
            STBIR_RGBA
        );

        width = StarTextureManager::targetResizeW;
        height = StarTextureManager::targetResizeH;

        imgReg = StarTextureManager::allocateRegion(width, height);

        //set uv coords
        uMin = imgReg.uMin;
        uMax = imgReg.uMax;
        vMin = imgReg.vMin;
        vMax = imgReg.vMax;


        int xOffset = (int)(imgReg.uMin * StarTextureManager::atlasWidth);
        int yOffset = (int)(imgReg.vMin * StarTextureManager::atlasHeight);
        
        glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, StarTextureManager::targetResizeW, StarTextureManager::targetResizeH, GL_RGBA, GL_UNSIGNED_BYTE, resized);
        glGenerateMipmap(GL_TEXTURE_2D);

        //std::cout << "Image resized to " << "(" << filePath  << "): " << width << "x" << height << "\n";


    }
    else
    {
        std::cout << "Failed to load texture at path: " << filePath << "\n";
    }

    stbi_image_free(data);
    free(resized);

    



}


StarTexture::~StarTexture(){}



