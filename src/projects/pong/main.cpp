#include <iostream>
#include "../core/Starisk.h"
// #include "../stariskEditor/temp.hpp"
#include <glad/glad.h>



extern "C" __declspec(dllexport) void runProject(Starisk* s)
{

    // Safety check to prevent Culprit 2
    if (!s) return;

    Entity& player = s->CreateEntity(50.0f, 10.0f, 30, 80);
    // setup only, no loop — editor drives the loop
}
    
