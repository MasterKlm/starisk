#include <iostream>
#include "ECS/components/KeyboardMovementComponent.h"
#include "ECS/systems/KeyboardMovementSystem.h"
#include "Starisk.h"


int main(){
	Starisk game;
	game.createWindow(1000, 800, "Test Starisk Lib");

	//auto player = game.CreateQuad(100, 100, 40, 80, glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));

	//	glfwSetKeyCallback(game.window, Keyboard::keyCallback);

	// std::function<void()> mainLoop = [&game](){
	//	if(Keyboard::key(GLFW_KEY_W)){
	//		std::cout << "W key" << "\n";
	//	}
		
	//};
	Entity& player = game.CreateEntity(300, 0, 50, 50, glm::vec4(0.0f,255.0f,0.0f,1.0f), 10.0f);
	player.addComponent<KeyboardMovementComponent>();

	game.addSystem<KeyboardMovementSystem>();
	
	std::function<void()> mainLoop = [&game](){

	};

	game.mainLoop(mainLoop);

	

    return 0;
}
