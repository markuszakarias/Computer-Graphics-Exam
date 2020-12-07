#pragma once

#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


class Shader {

private:

	int pointLightCount;
	int spotLightCount;

	// The GLuint for all of the uniform variables

	GLuint shaderID;
	GLuint uniformProjection;
	GLuint uniformView;
	GLuint uniformModel;
	GLuint uniformEyePosition;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;
	GLuint uniformMinimapTexture;
	GLuint uniformTexture;
	GLuint uniformTex;
	GLuint uniformDirectionalShadowMap;
	GLuint uniformDirectionalLightTransform;

	/* The structs are for the different types of light. The idea
	   here is simply to have the specific set of uniform variables
	   for each light, so that we don't end up with to many all in all.
	*/

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[15];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[3];

	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

public:

	Shader();

	void createShaderFromFile(const char* vertexLocation, const char* fragmentLocation);

	std::string readFile(const char* fileLocation);

	// The inline functions get uniforms from the shader

	inline GLuint getViewLocation() { return uniformView; }
	inline GLuint getModelLocation() { return uniformModel; }
	inline GLuint getShininessLocation() { return uniformShininess; }
	inline GLuint getProjectionLocation() { return uniformProjection; }
	inline GLuint getEyePositionLocation() { return uniformEyePosition; }
	inline GLuint getMinimapTextureLocation() { return uniformMinimapTexture; };
	inline GLuint getSpecularIntensityLocation() { return uniformSpecularIntensity; }
	inline GLuint getDirectionLocation() { return uniformDirectionalLight.uniformDirection; }
	inline GLuint getAmbientColourLocation() { return uniformDirectionalLight.uniformColour; }
	inline GLuint getAmbientIntensityLocation() { return uniformDirectionalLight.uniformAmbientIntensity; }
	inline GLuint getDiffuseIntensityLocation() { return uniformDirectionalLight.uniformDiffuseIntensity; }

	// Set the different lights during runtime

	void setDirectionalLight(std::shared_ptr<DirectionalLight>& dLight);
	void setPointLights(PointLight* pLight, unsigned int lightCount);
	void setSpotLights(SpotLight* sLight, unsigned int lightCount);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);

	void useShader();
	void unUseShader();
	void clearShader();

	void calculateAverageNormals(std::vector<GLuint> indices, unsigned int indiceCount, std::vector<GLfloat> vertices, unsigned int verticeCount,
		unsigned int vLength, unsigned int normalOffset);

	~Shader();

};