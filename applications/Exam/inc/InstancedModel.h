#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "Renderer.h"
#include "Model.h"

class InstancedModel
{
private:

	std::unique_ptr<Model> instancedModel;

	std::shared_ptr<VertexArray> instancedModelVAO;
	std::shared_ptr<VertexBuffer> instancedModelVBO;
	std::shared_ptr<VertexBufferLayout> instancedModelVBLayout;

	int numInstanced;

	std::vector<glm::vec3> instancedPositions;

	glm::mat4 projection;
	glm::mat4 model;
	glm::vec3 position;

	glm::mat4* modelMatrices;

	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

public:
	InstancedModel();
	~InstancedModel();

	InstancedModel(std::shared_ptr<Window>& mainWindow, std::vector <glm::vec3> positions);

	void generateInstanced();
	void drawInstanced(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& instanced_shader, glm::mat4 projection);

};