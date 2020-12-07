#pragma once

#include <iostream>
#include <GL/glew.h>
#include <stb_image.h>

class Material
{

private:

	GLfloat shininess;
	GLfloat specularIntensity;

	int width, height, bitDepth;

	const char* fileLocation;

public:

	Material();
	~Material();

	GLuint textureID;

	void getTexture(const char* fileLoc);
	void getSpecular(GLfloat sIntensity, GLfloat shine);

	bool loadTexture();
	bool loadTextureA();

	void useTexture();
	void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	void clearTexture();

};
