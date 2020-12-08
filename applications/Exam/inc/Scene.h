#pragma once

#include <iostream>
#include <set>
#include <cmath>
#include <vector>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <chrono>
#include <thread>

#include "Window.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Camera.h"
#include "ImportModel.h"
#include "InstancedModel.h"

#include "Material.h"
#include "Renderer.h"

#include "DirectionalLight.h"
#include "SpotLight.h"

class Scene {

private:

	GLfloat deltaTime;
	GLfloat lastTime;
	GLfloat time;
	GLfloat now;

	std::unique_ptr<Terrain> terrain;
	std::unique_ptr<ImportModel> import_trees;
	std::shared_ptr<InstancedModel> trees;

	std::shared_ptr<Renderer> renderer;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shader> instanceShader;
	std::shared_ptr<Shader> shadow;
	std::shared_ptr<Shader> dir_shadowmap;

	std::shared_ptr<Skybox> skybox;
	std::vector<std::string> skyboxFaces;

	std::shared_ptr<Camera> camera;
	std::shared_ptr<Window> mainWindow;
	std::shared_ptr<DirectionalLight> mapLight;

	SpotLight spotLights[3];

	glm::mat4 model;
	glm::mat4 projection;
	glm::mat4 skyboxViewMatrix;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;
	GLuint uniformEyePosition;

	GLuint uView;
	GLuint uProj;
	GLuint uMod;

	glm::vec3 terrain_pos;
	glm::vec3 start_pos;
	glm::vec3 model_pos;
	glm::vec3 lowerLight;

public:

	Scene();
	~Scene();

	void generateScene(std::shared_ptr<Window>& mainWindow);
	void updateScene(std::shared_ptr<Window>& mainWindow);

	void generateShaders();
	void generateLights();

	void updateLights();
	void updateTime();

};
