#pragma once

#include "Light.h"

/* -- The PointLight class has the Light class as it's parent. This makes it simpler       --
   -- for creating the lights in general, as the directional-and spot light class do the   --
   -- same. Some functionality is inherited.                                               --
   --							                                                           --
   -- This class get the diffuse and ambient values from the parent Class and sets the	   --
   -- position, constant, linear and exponent values.									   --
*/

class PointLight : public Light
{

//protected so the sub classes can access them.
protected:

	glm::vec3 position;

	GLfloat constant, linear, exponent;  // quadratic equation.

public:

	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	void useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	~PointLight();

};