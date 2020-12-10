#include "Scene.h"

/**
*   Constructor game object. This works essentially as "game engine startup" mechanism.
*
*/
Scene::Scene()
	:projection(0), start_pos(0), deltaTime(0), lastTime(0),
	time(0), now(0), model(1.0f), uniformModel(0),
	uniformProjection(0), uniformView(0) {

	start_pos = glm::vec3(500.0f, 100.0f, 500.0f);
	terrain_pos = glm::vec3(0.0f, -100.0f, 0.0f);

	numBirds = 20;
	generateBirdPositions();

	// Initial light colour and diffuse
	lightR = lightG = lightB = 1.f;
	diffuse = 1.2f;
	
	// Variables for linear interpolation in day/night cycle
	dt = interpolate = 0;
}

/**
*   Destructor for game object, everything is initialized as smart pointers.
*/
Scene::~Scene() {

}

/**
*   Generates the scene.
*
*   @see - generateShaders(), generateLights().
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
	trees = std::make_shared<InstancedModel>(mainWindow, terrain->getTreePositions());
	
	birds.resize(numBirds);

	for (size_t i = 0; i < numBirds; i++) {

		birds[i] = std::make_shared<ImportModel>(mainWindow, bird_pos[i]);
	}

	camera = std::make_shared<Camera>(start_pos, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 100.0f, 0.07f);
	projection = glm::perspective(glm::radians(45.0f), ((GLfloat)mainWindow->getBufferWidth() / mainWindow->getBufferHeight()), 0.1f, 2000.0f);

}

/**
*   Retrieves/makes/compiles all the shaders that will be used in the scene.
*
*   @see - createShaderFromFile()
*/
void Scene::generateShaders() {

	static const char* v_Shader = "assets/shaders/terrain.vert";
	static const char* f_Shader = "assets/shaders/terrain.frag";

	shader = std::make_shared<Shader>();
	shader->createShaderFromFile(v_Shader, f_Shader);

	static const char* v_instancedShader = "assets/shaders/instanced.vert";
	static const char* f_instancedShader = "assets/shaders/instanced.frag";

	instancedShader = std::make_shared<Shader>();
	instancedShader->createShaderFromFile(v_instancedShader, f_instancedShader);


	static const char* v_birdShader = "assets/shaders/birds.vert";
	static const char* f_birdShader = "assets/shaders/birds.frag";

	birdShader = std::make_shared<Shader>();
	birdShader->createShaderFromFile(v_birdShader, f_birdShader);
}

/**
*   Load/create all the lights that will be in the game
*/
void Scene::generateLights() {

	/* --- The Directional Lights in the scene --- */

	dayLight = std::make_shared<DirectionalLight>(
		lightR, lightG, lightB,			// Color
		0.08f, diffuse,					// Ambient, diffuse
		-2000.0f, -6400.0f, 200.0f);	// Direction

}

void Scene::generateBirdPositions() {

	for (size_t i = 0; i < numBirds; i++) {

		bird_pos.push_back(glm::vec3(rand() % 600, 79.f, rand() % 600));
	}
}

/**
*   Updating the scene as long as the window is open.
*	Function handles all event updating.
*
*   @param mainWindow - Current open window.
*   @see
*/
void Scene::updateScene(std::shared_ptr<Window>& mainWindow) {

	renderer->clear(0.1f, 0.1f, 0.1f, 1.0f);
	renderer->enableDepth();

	updateTime();
	camera->keyControls(mainWindow->retrieveKeys(), deltaTime);
	camera->mouseControl(mainWindow->getChangeX(), mainWindow->getChangeY());


	shader->useShader();
	updateDayNightCycle();
	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

	terrain->draw(model, terrain_pos, uniformModel);
	skybox->drawSkyBox(skyboxViewMatrix, projection, camera);
	
	birdShader->useShader();
	updateDayNightCycle();
	uniformModel = birdShader->getModelLocation();
	uniformProjection = birdShader->getProjectionLocation();
	uniformView = birdShader->getViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

	for (size_t i = 0; i < numBirds; i++) {

		birds[i]->drawModel(model, uniformModel);
		birds[i]->updateMovement(time);
	}

	instancedShader->useShader();
	updateDayNightCycle();
	trees->drawInstanced(camera, instancedShader, projection);
}

/**
* Updates lights
*
* @see - setDirectionalLight().
*
*/
void Scene::updateLights() {

	shader->setDirectionalLight(dayLight);

}

/**
* Updating every birds position
*/
void Scene::updateBirdPos() {

	for (size_t i = 0; i < numBirds; i++) {

		birds[i]->updateMovement(time);
	}
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
* Updates the directional light which is used as the sun in the scene
* and setting the colour based on the time of scene.
*/
void Scene::updateDayNightCycle() {

	lightR = (1 - interpolate) * 1.f + interpolate * 0.36f;
	lightG = (1 - interpolate) * 1.f + interpolate * 0.46f;
	lightB = (1 - interpolate) * 1.f + interpolate * 0.5f;
	diffuse = (1 - interpolate) * 1.2f + interpolate * 0.4f;

	if (interpolate <= 0) {
		dt = 1.0f / 1200;
	}
	
	if (interpolate >= 1) {
		dt = -1.0f / 1200;
	}
	
	interpolate += dt;

	dayLight = std::make_shared<DirectionalLight>(
		lightR, lightG, lightB,			// Color
		0.08f, diffuse,					// Ambient, diffuse
		-2000.0f, -6400.0f, 200.0f);	// Direction

	shader->setDirectionalLight(dayLight);

}