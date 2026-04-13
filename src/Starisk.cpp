#include "Starisk.h"


Starisk::Starisk() {}

void Starisk::init(){}


StarQuad Starisk::CreateQuad(float x_pos, float y_pos, float width, float height, const char* texturePath)
{
    StarQuad quad{x_pos, y_pos, width, height, texturePath};
    sbm.add(quad.vertices);

    return quad;
}

StarQuad Starisk::CreateQuad(float x_pos, float y_pos, float width, float height, glm::vec4 rgba)
{
    StarQuad quad{x_pos, y_pos, width, height, glm::vec4(rgba)};
    sbm.add(quad.vertices);

    return quad;
}

void Starisk::mainLoop(std::function<void()> fn)
{
    sbm.update();
    fn();
}

void Starisk::setGameRunning(bool v)
{
    gameRunning = v;
}

bool Starisk::isGameRunning(){ return gameRunning; }




Starisk::~Starisk(){}