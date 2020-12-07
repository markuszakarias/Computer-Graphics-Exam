#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <Shader.h>
#include <VertexArray.h>
#include <VertexBufferLayout.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Renderer.h>
#include <Camera.h>


class Skybox {
private:

	std::shared_ptr<Renderer> skyboxRenderer;

	std::shared_ptr<Shader> skyboxShader;

	std::shared_ptr<VertexArray>		skyboxVAO;
	std::unique_ptr<VertexBuffer>		skyboxVBO;
	std::unique_ptr<VertexBufferLayout> skyboxVBLayout;
	std::shared_ptr<IndexBuffer>		skyboxIBO;

	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;

	GLuint textureID;
	GLuint uniformView, uniformProjection;

	int width, height, bitdepth;

public:

	Skybox();
	Skybox(std::vector<std::string> faceLocations);

	void drawSkyBox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, std::shared_ptr<Camera> camera);

	~Skybox();



};
