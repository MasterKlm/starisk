#include <iostream>
#include "../../core/Starisk.h"


int main()
{

    Starisk starisk;
    starisk.createWindow(800, 600, "Testing Lib");

    auto& player = starisk.CreateEntity(100.0f, 200.0f, 30, 80);

    starisk.mainLoop();
    return 0;
}