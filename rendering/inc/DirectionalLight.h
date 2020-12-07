#pragma once

#include <iostream>

#include "Light.h"

/* -- The DirectionalLight class has the Light class as it's parent. This makes it simpler -- 
   -- for creating the lights in general, as the spot-and point light do the same. some    --
   -- functionality is inherited.                                                          --
   --							                                                           --
   -- This class get the diffuse and ambient values from the parent Class and sets the	   --
   -- direction only.																	   --
*/

class DirectionalLight : public Light
{

private:

	glm::vec3 direction;

public:

	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir);

	DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, 
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	glm::mat4 CalculateLightTransform();

	~DirectionalLight();

};
