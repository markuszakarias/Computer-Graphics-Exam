#include <stb_image.h>
#include "Terrain.h"

Terrain::Terrain() {

	// Loading the height map
	loadTerrainFromMap("assets/mapping/HeightMaps/Randsf_HeightMap.png");

}

Terrain::~Terrain() {

}

void Terrain::loadTerrainFromMap(const std::string filename) {

	/* Source:
		http://www.lighthouse3d.com/opengl/terrain/index.php?heightmap */

	int numberOfChnls;
	stbi_uc *map = stbi_load(filename.c_str(), &width, &height, &numberOfChnls, 0);

	heightMap.resize(height, std::vector<int>(width));

	if (map != nullptr) {
		
		int num = 0;

		for (int z = 0; z < height; z++) {
			for (int x = 0; x < width; x++) {

				heightMap[z][x] = static_cast<int>(map[num * numberOfChnls]);
				num++;
			}
		}
	}
	else {

		std::cout << "Error loading height map" << std::endl;
	}

	stbi_image_free(map);

	for (int z = 0; z < heightMap.size(); z++) {
		for (int x = 0; x < heightMap[0].size(); x++) {

			GLfloat UV1, UV2;

			if (z % 2 == 0) {
				if (x % 2 == 0) {
					UV1 = 0.f;
					UV2 = 0.f;
				}
				else {
					UV1 = 0.f;
					UV2 = 1.f;
				}
			}
			else {
				if (x % 2 == 0) {
					UV1 = 1.f;
					UV2 = 0.f;
				}
				else {
					UV1 = 1.f;
					UV2 = 1.f;
				}
			}


			GLfloat xCoord = x;
			GLfloat yCoord = heightMap[z][x];
			GLfloat zCoord = z;

			std::vector <GLfloat> tempVertex = {
				xCoord, yCoord, zCoord,
				UV1, UV2,
				xCoord, -yCoord, zCoord 
			};

			vertices.insert(end(vertices), tempVertex.begin(), tempVertex.end());

		}
	}

	for (GLuint z = 0; z < height - 1; z++) {
		for (GLuint x = 0; x < width - 1; x++) {
			if (x == 5) {
				x++;
			}

			std::vector<GLuint> tempIndex = {
				x + width * z, width * z + x + 1, x + (z + 1) * width,
				width * z + x + 1, (z + 1) * width + x, (z + 1) * width + x + 1
			};
			indices.insert(end(indices), tempIndex.begin(), tempIndex.end());
		}
	}

	//terrainShader->calculateAverageNormals(indices, indices.size(), vertices, vertices.size(), 8, 5);
	
	terrainVAO = std::make_shared<VertexArray>();
	terrainVAO->bind();

	terrainVBO = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(GLfloat));
	terrainVBO->bind();

	terrainVBLayout = std::make_unique<VertexBufferLayout>();
	terrainVBLayout->Push<float>(3);
	terrainVBLayout->Push<float>(2);
	terrainVBLayout->Push<float>(3);

	terrainVAO->addBuffer(*terrainVBO, *terrainVBLayout);
	
	terrainIBO = std::make_shared<IndexBuffer>(indices.data(), indices.size());

	terrainMat = std::make_unique<Material>();
	terrainMat->getTexture("assets/textures/mountain.png");
	terrainMat->loadTexture();

}

void Terrain::generateShader() {

	static const char* vShader = "assets/shaders/terrain.vert";
	static const char* fShader = "assets/shaders/terrain.frag";

	terrainShader = std::make_unique<Shader>();
	terrainShader->createShaderFromFile(vShader, fShader);

}

void Terrain::draw(glm::mat4 model, glm::vec3 position, GLuint uniformModel) {

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	terrainMat->useTexture();
	terrainRenderer->drawElements(terrainVAO, terrainIBO);

	/*
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.0f, 0.5f, 25.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	importModel->renderElements();
	*/
}