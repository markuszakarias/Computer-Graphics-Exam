#include "Scene.h"

/**
*   Constructor game object. This works essentially as "game engine startup" mechanism.
*
*/
Scene::Scene()
	:projection(0), start_pos(0), deltaTime(0), lastTime(0),
	time(0), now(0), model(1.0f), lowerLight(0), uniformModel(0),
	uniformProjection(0), uniformView(0) {

	start_pos = glm::vec3(1.0f, 10.0f, 1.0f);
	terrain_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	model_pos = glm::vec3(10.0f, 0.5f, 35.0f);
}

/**
*   Destructor for game object, everything is initialized as smart pointers.
*/
Scene::~Scene() {

}

/**
*   retrieves/makes/compiles all the shaders that will be used in the entire game.
*
*   @see - createShaderFromFile()
*/
void Scene::generateShaders() {

	static const char* vShader = "assets/shaders/lights.vert";
	static const char* fShader = "assets/shaders/lights.frag";

	shader = std::make_shared<Shader>();
	shader->createShaderFromFile(vShader, fShader);
}

/**
*   Load/create all the lights that will be in the game
*
*/
void Scene::generateLights() {

	/* --- The Directional Lights in the scene --- */

	mapLight = std::make_shared<DirectionalLight>(
		1.0f, 1.0f, 1.0f,	 // Color
		0.2f, 0.5f,			 // Ambient, diffuse 
		5.0f, -15.0f, 6.0f); // Direction
}

/**
*   Generate the entire game. All objects, all needed functions.
*
*   @see - generateShaders(), generateLights(), generateTextures(), getLevelArray(), getBufferWidth(), getBufferHeight().
*/
void Scene::generateScene(std::shared_ptr<Window>& mainWindow) {

	generateShaders();
	generateLights();

	skyboxFaces.push_back("assets/textures/skybox_1/right.png");
	skyboxFaces.push_back("assets/textures/skybox_1/left.png");
	skyboxFaces.push_back("assets/textures/skybox_1/top.png");
	skyboxFaces.push_back("assets/textures/skybox_1/bottom.png");
	skyboxFaces.push_back("assets/textures/skybox_1/front.png");
	skyboxFaces.push_back("assets/textures/skybox_1/back.png");

	skybox = std::make_shared<Skybox>(skyboxFaces);

	terrain = std::make_unique<Terrain>();
	camera = std::make_shared<Camera>(start_pos, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 4.0f, 0.07f);
	projection = glm::perspective(glm::radians(45.0f), ((GLfloat)mainWindow->getBufferWidth() / mainWindow->getBufferHeight()), 0.1f, 1200.0f);
}

/**
*   Updates lights
*
*   @see - setDirectionalLight(), setSpotLights(), getCameraPosition(), setFlash(), getCameraDirection().
*
*/
void Scene::updateLights() {

	shader->setDirectionalLight(mapLight);

	shader->setSpotLights(spotLights, 1);
	lowerLight = camera->getCameraPosition();
	lowerLight.y -= 0.1f; // Offsets the flashlight
	spotLights[0].setFlash(lowerLight, camera->getCameraDirection());
}

/**
*	
*	Checks for the collision of the pellets within certain time intervals.
*	
*/
void Scene::updateTime() {

	now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	time += deltaTime;
}

/**
*   Updating the game as long as the window is open.
*	Function handles all event updating.
*
*   @param mainWindow - Current open window.
*   @see   useShader(), updateMVP(), updateTime(), keyControls(), mouseControl(), updateLights(),
*		   retrieveKeys(), toggleFlashLight(), clear(), enableDepth(), draw(), checkCameraCollision(),
*		   closeWindow(), getViewLocation(), getProjectionLocation(), calculateViewMatrix(),
*		   setDirectionalLight(), setSpotLights(), allPelletsEaten(), closeWindow(), updateMinimap().
*/
void Scene::updateScene(std::shared_ptr<Window>& mainWindow) {

	renderer->clear(0.1f, 0.1f, 0.1f, 1.0f);
	renderer->enableDepth();

	updateTime();
	camera->keyControls(mainWindow->retrieveKeys(), deltaTime);
	camera->mouseControl(mainWindow->getChangeX(), mainWindow->getChangeY());

	shader->useShader();
	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

	shader->setDirectionalLight(mapLight);

	terrain->draw(model, terrain_pos, uniformModel);

	skybox->drawSkyBox(skyboxViewMatrix, projection, camera);
}