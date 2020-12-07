#include "Scene.h"

/**
* Autumn 2020 Exam PROG2002.
* 
* @name Terrain.exe
* @author Markus Zakarias Faljord Strømseth, NTNU
*/
int main() {

	auto mainWindow = std::make_shared<Window>(800, 600);
	mainWindow->initialise();

	auto terrainScene = std::make_unique<Scene>();

	while (!mainWindow->shouldClose()) {

		glfwPollEvents();
		//renderloop
		mainWindow->swapBuffer();
	}

	return EXIT_SUCCESS;
}