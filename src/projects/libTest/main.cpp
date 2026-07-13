#include <iostream>
#include "../../core/Starisk.h"


int main()
{

    Starisk starisk;
    starisk.createWindow(800, 600, "Testing Lib");

    auto& player = starisk.CreateEntity(100.0f, 200.0f, 30, 80);
    auto& player2 = starisk.CreateEntity(07.0f, 20.0f, 30, 80);
    auto& player3 = starisk.CreateEntity(10.0f, 270.0f, 30, 80);
    auto& player4 = starisk.CreateEntity(100.0f, 60.0f, 30, 80);

    starisk.mainLoop([&starisk](){
        std::cout << "entities: " << starisk.sem.entityCount << "\n";
    });

    return 0;
}