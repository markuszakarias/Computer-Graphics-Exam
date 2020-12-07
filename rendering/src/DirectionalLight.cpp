#include "DirectionalLight.h"

/**
*  The DirectionalLight class has the Light class as it's parent. This makes it simpler 
*  for creating the lights in general, as the spot-and point light do the same. some     
*  functionality is inherited.                                                          
*   							                                                           
*  This class get the diffuse and ambient values from the parent Class and sets the	   
*  direction only.																	   
*
*  @name DirectionalLight.cpp
*  @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*  DirectionalLight default constructor. Derives from the Light class. 
*
*/
DirectionalLight::DirectionalLight() : Light() {

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

/**
* 
*  DirectionalLight constructor. This is used when setting the light. Derives from
*  the Light class. 
*   
*  @param	red			- RGB red color
*  @param	green		- RGB green color
*  @param	blue		- RGB blue color
*  @param	aIntensity	- Ambient intensity
*  @param	dIntensity	- Diffuse intensity
*  @param	xDir		- The light x-axis direction
*  @param	yDir		- The light y-axis direction
*  @param	zDir		- The light z-axis direction
*  
*  @see Light()
*
*/
DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir)
	: Light(red, green, blue, aIntensity, dIntensity) {

	direction = glm::vec3(xDir, yDir, zDir);
}

/**
*
*  DirectionalLight constructor. This is used when setting the light. Derives from
*  the Light class.
*
*  @param	shadowWidth	 - The shadowmap width
*  @param	shadowHeight - The shadowmap heigth
*  @param	red			 - RGB red color
*  @param	green		 - RGB green color
*  @param	blue		 - RGB blue color
*  @param	aIntensity	 - Ambient intensity
*  @param	dIntensity	 - Diffuse intensity
*  @param	xDir		 - The light x-axis direction
*  @param	yDir		 - The light y-axis direction
*  @param	zDir		 - The light z-axis direction
*
*  @see Light()
*
*/
DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, 
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir)
	: Light(2048, 2048, red, green, blue, aIntensity, dIntensity) {

	direction = glm::vec3(xDir, yDir, zDir);

	lightProj = glm::ortho(-60.0f, 60.0f, -60.0f, 60.0f, 0.1f, 500.0f);
}

/**
* 
*  This function binds passed parameters to the shader.
* 
*  @param	ambientIntensityLocation - The ambient intensity location in the shader
*  @param	ambientColourLocation	 - The ambient color location in the shader
*  @param	diffuseIntensityLocation - The diffuse intensity location in the shader
*  @param	directionLocation		 - The direction location in the shader
*
*/
void DirectionalLight::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation) {

	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

/**
*
*	PLACEHOLDER
*
*/
glm::mat4 DirectionalLight::CalculateLightTransform() {

	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

/**
*  Destructor of the DirectionalLight.
*/
DirectionalLight::~DirectionalLight() {

}