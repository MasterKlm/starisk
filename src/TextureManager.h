#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

struct AtlasRegion {
    float uMin = 0.0f, vMin = 0.0f, uMax = 0.0f, vMax = 0.0f;
};

class TextureManager
{

public:
    GLuint atlasID;
    static int atlasWidth, atlasHeight;
    static int targetResizeW , targetResizeH;
    static int currentX, currentY, rowHeight;

    TextureManager();
    ~TextureManager();

    static AtlasRegion getRegion(int xOffset, int yOffset, int width, int height);
    static AtlasRegion allocateRegion(int width, int height);
    void Bind();
};


#endif