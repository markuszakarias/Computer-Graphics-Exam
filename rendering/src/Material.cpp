#include "Material.h"

/**
*  The Material class gets specular and shine values from the shader and adds them to the surface of an object.
*
*  @name Material.cpp
*  @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*	Material class default constructor. Initialize values to zero.
*
*/
Material::Material()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

/**
*	Gets specular values to apply to the surface of the object.
*
*   @param	sIntensity - The specular intensity on the surface
*   @param	shine	   - The shine effect on the surface
*/

void Material::getSpecular(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::getTexture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;

}

bool Material::loadTexture()
{
	stbi_set_flip_vertically_on_load(1); // Comment this to not flip texture - will be beneficial most of the time
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		std::cout << "Failed to find texture at location: " << fileLocation << std::endl;
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	return true;
}

bool Material::loadTextureA()
{
	//stbi_set_flip_vertically_on_load(1); // Comment this to not flip texture - will be beneficial most of the time
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		std::cout << "Failed to find texture at location: " << fileLocation << std::endl;
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	return true;
}

void Material::useTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Material::clearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

/**
*	Applies the material to the assigned object. Passes the values to the shader
*
*   @param	specularIntensityLocation	- Bind the specular intensity to the shader
*   @param	shininessLocation			- Bind the shininess to the shader
*/
void Material::useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}

/**
*	Material destructor
*
*/
Material::~Material()
{
	clearTexture();
}