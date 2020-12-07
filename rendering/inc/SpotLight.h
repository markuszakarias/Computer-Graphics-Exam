#pragma once

#include "PointLight.h"

/* -- The PointLight class has the PointLight class as it's parent. This is because both   --
   -- of the lights are to some degree similar. You can say that the spotlight is a more   --
   -- focused point light, with some fall-off. This can also be used to create some         --
   -- flashlight effects, which is what we have done here to the floor object.             --
   --							                                                           --
   -- This class gets the colors, diffuse, ambient, positions, constant, linear and  	   --
   -- exponent values from the parent Class and sets the direction and edge values.		   --
*/

class SpotLight : public PointLight
{

private:

	glm::vec3 direction;

	GLfloat edge, procEdge;

	bool isOn;

public:

	SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg);

	void useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);

	void setFlash(glm::vec3 pos, glm::vec3 dir);

	void toggleFlashLight() { if (isOn) isOn = false; else isOn = true; }

	~SpotLight();

};