#ifndef STARISK_H
#define STARISK_H
#include <iostream>
#include "glm/glm.hpp"
#include "glm/common.hpp"
#include "StarQuad.h"
#include "StarBatchManager.h"
#include <functional>


class Starisk
{
public:

    StarBatchManager sbm{};
    
    Starisk();
    ~Starisk();
    
    StarQuad CreateQuad(float x_pos, float y_pos, float weight, float height, const char* texturePath);
    StarQuad CreateQuad(float x_pos, float y_pos, float weight, float height, glm::vec4 rgba);
    
    void mainLoop(std::function<void()> fn);
    
    void init();

    void setGameRunning(bool v);
    bool isGameRunning();
private:
    bool gameRunning = true;
};

#endif