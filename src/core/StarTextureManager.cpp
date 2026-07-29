#include "StarTextureManager.h"

int StarTextureManager::currentX = 0;
int StarTextureManager::currentY = 0;
int StarTextureManager::rowHeight = 0;
int StarTextureManager::atlasWidth = 8000, StarTextureManager::atlasHeight = 8000;
int StarTextureManager::targetResizeW = 64, StarTextureManager::targetResizeH = 64;
//125 total available texture slots

StarTextureManager::StarTextureManager()
{
    //GLint maxSize;
    //glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
    //std::cout << "Max texture size: " << maxSize << "\n";

    glGenTextures(1, &atlasID);
    glBindTexture(GL_TEXTURE_2D, atlasID);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "Atlas creation failed with GL error: " << err << "\n";
    }
}

StarTextureManager::~StarTextureManager(){}

void StarTextureManager::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasID);
}

AtlasRegion StarTextureManager::getRegion(int xOffset, int yOffset, int width, int height) {
    return {
        (float)xOffset / atlasWidth,
        (float)yOffset / atlasHeight,
        (float)(xOffset + width) / atlasWidth,
        (float)(yOffset + height) / atlasHeight
    };
}


AtlasRegion StarTextureManager::allocateRegion(int width, int height) {
    // if this texture doesn't fit on current row, start a new row
    if (currentX + width > atlasWidth) {
        currentX = 0;
        currentY += rowHeight;
        rowHeight = 0;
    }

    AtlasRegion region = getRegion(currentX, currentY, width, height);

    currentX += width;
    if (height > rowHeight) rowHeight = height;

    return region;
}


