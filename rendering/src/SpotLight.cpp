#include "SpotLight.h"


/**
*   The SpotLight class has the PointLight class as it's parent. This is because both   
*   of the lights are to some degree similar. You can say that the spotlight is a more  
*   focused point light, with some fall-off. This can also be used to create some        
*   flashlight effects, which is what we have done in this application.           
*  							                                                          
*   This class gets the colors, diffuse, ambient, positions, constant, linear and  	  
*   exponent values from the parent class and sets the direction and edge values.		   
*
*	@name SpotLight.cpp
*	@author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*	Default constructor for the SpotLight. Derives from the parent class PointLight.
*
*/
SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}

/**
*	Constructor for the SpotLight. Derives from the parent class PointLight.
*
*  @param	red			- RGB red color
*  @param	green		- RGB green color
*  @param	blue		- RGB blue color
*  @param	aIntensity	- Ambient intensity
*  @param	dIntensity	- Diffuse intensity
*  @param	xPos		- The light position in the x-axis
*  @param	yPos		- The light position in the y-axis
*  @param	zPos		- The light position in the z-axis
*  @param	xDir		- The light direction in the z-axis
*  @param	yDir		- The light direction in the z-axis
*  @param	zDir		- The light direction in the z-axis
*  @param	con			- The lights constant
*  @param	lin			- The lights linear
*  @param	exp			- The lights exponent
*  @param	edg			- The light circle size (flashlight size)
*
*/
SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat xDir, GLfloat yDir, GLfloat zDir,
	GLfloat con, GLfloat lin, GLfloat exp,
	GLfloat edg) : PointLight(red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));

	edge = edg;
	procEdge = cosf(glm::radians(edge));

	isOn = false;
}

/**
*  This function binds passed parameters to the shader.
*
*  @param	ambientIntensityLocation - The ambient intensity location in the shader.
*  @param	ambientColourLocation	 - The ambient color location in the shader.
*  @param	diffuseIntensityLocation - The diffuse intensity location in the shader.
*  @param	positionLocation		 - The position location in the shader.
*  @param	directionLocation		 - The direction location in the shader.
*  @param	constantLocation		 - The constant location in the shader.
*  @param	linearLocation			 - The linear location in the shader.
*  @param	exponentLocation		 - The exponent location in the shader.
*  @param	edgeLocation			 - The edge location in the shader.
*
*/
void SpotLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
	GLuint edgeLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);

	// Condition to enable/disable flashlight
	if (isOn)
	{
		glUniform1f(ambientIntensityLocation, ambientIntensity);
		glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	}

	else 
	{
		glUniform1f(ambientIntensityLocation, 0.0f);
		glUniform1f(diffuseIntensityLocation, 0.0f);
	}
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

/**
*	Sets the flashlight effect.
*
*   @param pos	-	The position of the flashlight.
*   @param dir	-	The direction of the flashlight.
*/
void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

/**
*	Destructor for the SpotLight
*
*/
SpotLight::~SpotLight()
{

}