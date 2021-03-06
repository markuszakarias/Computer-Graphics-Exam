#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>

#include "DirectionalLight.h"

#include "Window.h"
#include "Shader.h"
#include "Camera.h"

#include "Material.h"
#include "Renderer.h"

#include "Model.h"

class ImportModel
{

private:

	std::unique_ptr<Model> importModel;

	std::shared_ptr<VertexArray> importModelVAO;
	std::shared_ptr<VertexBuffer> importModelVBO;
	std::shared_ptr<VertexBufferLayout> importModelVBLayout;

	glm::mat4 projection;
	glm::mat4 model;
	glm::vec3 position;

	std::vector <glm::vec3> velocities;

public:
	ImportModel();
	~ImportModel();

	ImportModel(std::shared_ptr<Window>& mainWindow, glm::vec3 pos);

	void generateModel();

	void updateMovement(GLfloat dt);

	void drawModel(glm::mat4 model, GLuint uniformModel);

};