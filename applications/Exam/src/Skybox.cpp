#include "Skybox.h"

/**
* 
*	PLACEHOLDER
*
*/
Skybox::Skybox() {}

/**
*
*	PLACEHOLDER
*
*/
Skybox::~Skybox() {}

/**
*
*	PLACEHOLDER
*
*/
Skybox::Skybox(std::vector<std::string> faceLocations) {

	static const char* skyboxV = "assets/shaders/skybox.vert";
	static const char* skyboxF = "assets/shaders/skybox.frag";

	skyboxShader = std::make_shared<Shader>();
	skyboxShader->createShaderFromFile(skyboxV, skyboxF);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (size_t i = 0; i < 6; i++) {
		unsigned char* texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitdepth, 0);
		if (!texData) {
			std::cout << "Could not retrieve the facelocation(s) ---> " << faceLocations[i].c_str();
			return;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	std::vector<GLfloat> verticePlaceHolder = {
		-1000.0f,  1000.0f, -1000.0f,		-1000.0f, -1000.0f,		-1000.0f, -1000.0f, -1000.0f,
		-1000.0f, -1000.0f, -1000.0f,		-1000.0f, -1000.0f,		-1000.0f, -1000.0f, -1000.0f,
		 1000.0f,  1000.0f, -1000.0f,		-1000.0f, -1000.0f,		-1000.0f, -1000.0f, -1000.0f,
		 1000.0f, -1000.0f, -1000.0f,		-1000.0f, -1000.0f,		-1000.0f, -1000.0f, -1000.0f,
				 
		-1000.0f,  1000.0f,  1000.0f,		-1000.0f, -1000.0f,		-1000.0f, -1000.0f, -1000.0f,
		 1000.0f,  1000.0f,  1000.0f,		-1000.0f, -1000.0f,		-1000.0f, -1000.0f, -1000.0f,
		-1000.0f, -1000.0f,  1000.0f,		-1000.0f, -1000.0f,		-1000.0f, -1000.0f, -1000.0f,
		 1000.f, - 1000.0f,  1000.0f,		-1000.0f, -1000.0f,		-1000.0f, -1000.0f, -1000.0f
	};

	vertices.insert(end(vertices), verticePlaceHolder.begin(), verticePlaceHolder.end());

	std::vector<GLuint> indicesPlaceHolder = {
		
		0, 1, 2,
		2, 1, 3,

		2, 3, 5,
		5, 3, 7,

		5, 7, 4,
		4, 7, 6,

		4, 6, 0,
		0, 6, 1,

		4, 0, 5,
		5, 0, 2,

		1, 6, 3,
		3, 6, 7
	};


	indices.insert(end(indices), indicesPlaceHolder.begin(), indicesPlaceHolder.end());

	skyboxVAO = std::make_shared<VertexArray>();
	skyboxVAO->bind();

	skyboxVBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(GLfloat));
	skyboxVBO->bind();

	skyboxVBLayout = std::make_unique<VertexBufferLayout>();
	skyboxVBLayout->Push<float>(3);
	skyboxVBLayout->Push<float>(2);
	skyboxVBLayout->Push<float>(3);

	skyboxVAO->addBuffer(*skyboxVBO, *skyboxVBLayout);
	skyboxIBO = std::make_shared<IndexBuffer>(&indices[0], indices.size());
}

/**
*
*	PLACEHOLDER
*
*/
void Skybox::drawSkyBox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, std::shared_ptr<Camera> camera) {

	glDepthMask(GL_FALSE);

	skyboxShader->useShader();

	uniformProjection = skyboxShader->getProjectionLocation();
	uniformView = skyboxShader->getViewLocation();

	viewMatrix = camera->calculateViewMatrix();

	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	skyboxRenderer->drawElements(skyboxVAO, skyboxIBO);
	glDepthMask(GL_TRUE);
}
