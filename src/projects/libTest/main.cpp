#include <iostream>
#include "../../core/Starisk.h"


int main()
{

    Starisk starisk;
    starisk.createWindow(800, 600, "Testing Lib");

    auto& player = starisk.CreateEntity(100.0f, 200.0f, 30, 80);
   
    StarVec2D point1 = StarVec2D(10.0f, 60.0f);
    StarVec2D point2 = StarVec2D(100.0f, 20.0f);

    starisk.sbm->drawLine(point1, point2, 800, 600);
    point1 += StarVec2D(50.0f, 50.0f);
    point2 += StarVec2D(50.0f, -50.0f);
    starisk.sbm->drawLine(point1, point2, 800, 600);

    starisk.mainLoop([&starisk](){
    });

    return 0;
}