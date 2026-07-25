#include <iostream>
#include "../../core/Starisk.h"


int main()
{

    Starisk starisk;
    starisk.createWindow(800, 600, "Testing Lib");

    auto& player = starisk.CreateEntity(100.0f, 200.0f, 30, 80);
   
    StarLine line = starisk.CreateLine(10.0f, 20.0f, 400.0f, 280.0f);

    starisk.mainLoop([&starisk](){
    });

    return 0;
}