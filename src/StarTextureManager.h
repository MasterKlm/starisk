#ifndef STARTEXTUREMANAGER_H
#define STARTEXTUREMANAGER_H
#include "StariskExport.h"
#include <unordered_map>
#include <string>

struct STARISK_API AtlasRegion {
    float uMin = 0.0f, vMin = 0.0f, uMax = 0.0f, vMax = 0.0f;
};

class STARISK_API StarTextureManager
{

public:
    GLuint atlasID;
    static int atlasWidth, atlasHeight;
    static int targetResizeW , targetResizeH;
    static int currentX, currentY, rowHeight;

    StarTextureManager();
    ~StarTextureManager();

    static AtlasRegion getRegion(int xOffset, int yOffset, int width, int height);
    static AtlasRegion allocateRegion(int width, int height);
    bool hasLoadedTexture(std::string filePath);
    void Bind();
private:
    std::unordered_map<std::string, AtlasRegion> loadedTextures;
};


#endif