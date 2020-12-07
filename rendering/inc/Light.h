#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.h"

/* -- The parent class which has functionality that the DirectionalLight,   --
*  -- SpotLight and PointLight class inherits from. Could be an idea to     --
*  -- have all the functions regarding the lights in here, but for now they --
*  -- are separate.															--
*/

class Light
{

//These are protected so that the sub classes can access them
protected:

	glm::vec3 colour;

	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;

public:

	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
	Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
	
	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);

	ShadowMap* getShadowMap() { return shadowMap; }

	~Light();

};