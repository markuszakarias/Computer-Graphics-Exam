#include "Scene.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <dr_wav.h>
#include <Audio.h>

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

	auto audioengine = std::make_unique<Audio>();
	audioengine->getHostDevice();
	audioengine->createContext();
	audioengine->createListener();
	audioengine->initializeStruct();
	audioengine->generateStereoBuffer();
	ALint sourceS = audioengine->getSourceState();
	ALint StereoS = audioengine->getStereoSource();
	alec(alSourcePlay(StereoS));
	alec(alGetSourcei(StereoS, AL_SOURCE_STATE, &sourceS));

	while (!mainWindow->shouldClose()) {

		glfwPollEvents();
		terrainScene->updateScene(mainWindow);
		mainWindow->swapBuffer();
	}

	return EXIT_SUCCESS;
}