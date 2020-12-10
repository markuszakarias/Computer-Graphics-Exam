#include "Scene.h"

/**
* Autumn 2020 Exam PROG2002.
* 
* @name Terrain.exe
* @author Markus Zakarias Faljord Strømseth, NTNU
*/
int main() {

	auto mainWindow = std::make_shared<Window>(1400, 1000);
	mainWindow->initialise();

	auto terrainScene = std::make_unique<Scene>();
	terrainScene->generateScene(mainWindow);

	while (!mainWindow->shouldClose()) {

		glfwPollEvents();
		terrainScene->updateScene(mainWindow);
		mainWindow->swapBuffer();
	}

	return EXIT_SUCCESS;
}