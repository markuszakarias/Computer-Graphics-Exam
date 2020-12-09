#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "Renderer.h"
#include "Model.h"

class Water {

private:

	int width, height;

	std::vector <GLfloat> vertices;
	std::vector <GLuint> indices;

	std::shared_ptr<VertexArray>		waterVAO;
	std::unique_ptr<VertexBuffer>		waterVBO;
	std::unique_ptr<VertexBufferLayout> waterVBLayout;
	std::shared_ptr<IndexBuffer>		waterIBO;

	std::unique_ptr<Shader>				waterShader;
	std::unique_ptr<Material>			waterMat;
	std::unique_ptr<Renderer>			waterRenderer;

	GLuint uniformView;
	GLuint uniformProjection;


public:
	Water();
	~Water();
	void generateShader();
	void draw(glm::mat4 model, glm::vec3 position, GLuint uniformModel);

};