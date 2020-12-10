#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "Renderer.h"
#include "Model.h"

class Terrain {

private:

	int width, height;
	std::vector <std::vector <int>> heightMap;
	std::vector <glm::vec3> treePositions;

	std::vector <GLfloat> vertices;
	std::vector <GLuint> indices;

	std::shared_ptr<VertexArray>			terrainVAO;
	std::unique_ptr<VertexBuffer>			terrainVBO;
	std::unique_ptr<VertexBufferLayout>		terrainVBLayout;
	std::shared_ptr<IndexBuffer>			terrainIBO;

	std::unique_ptr<Shader>					terrainShader;
	std::unique_ptr<Material>				terrainMat;
	std::unique_ptr<Renderer>				terrainRenderer;

	GLuint uniformView;
	GLuint uniformProjection;


public:
	Terrain();
	~Terrain();
	void loadTerrainFromMap(const std::string filename);
	void generateShader();
	float getTileHeight(float objx, float objz);
	std::vector <glm::vec3> getTreePositions();
	void draw(glm::mat4 model, glm::vec3 position, GLuint uniformModel);
	
};