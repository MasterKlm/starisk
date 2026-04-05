#include "TextureManager.h"

int TextureManager::currentX = 0;
int TextureManager::currentY = 0;
int TextureManager::rowHeight = 0;
int TextureManager::atlasWidth = 8000, TextureManager::atlasHeight = 8000;
int TextureManager::targetResizeW = 64, TextureManager::targetResizeH = 64;
//125 total available texture slots

TextureManager::TextureManager()
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

TextureManager::~TextureManager(){}

void TextureManager::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasID);
}

AtlasRegion TextureManager::getRegion(int xOffset, int yOffset, int width, int height) {
    return {
        (float)xOffset / atlasWidth,
        (float)yOffset / atlasHeight,
        (float)(xOffset + width) / atlasWidth,
        (float)(yOffset + height) / atlasHeight
    };
}


AtlasRegion TextureManager::allocateRegion(int width, int height) {
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