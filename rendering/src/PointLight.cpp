#include "PointLight.h"

 
/**
*	The PointLight class has the Light class as it's parent. This makes it simpler       
*   for creating the lights in general, as the directional-and spotlight class do the     
*   same. Some functionality is inherited.                                               
*  							                                                           
*   This class get the diffuse and ambient values from the parent Class and sets the	   
*   position, constant, linear and exponent values.									   
* 
*   @name PointLight.cpp
*   @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*	Default constructor for the PointLight. Derives from the parent class Light.
*
*/
PointLight::PointLight() : Light() {

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

/**
*	Constructor for the PointLight. Derives from the parent class Light, with shadows.
*
*  @param	red			- RGB red color
*  @param	green		- RGB green color
*  @param	blue		- RGB blue color
*  @param	aIntensity	- Ambient intensity
*  @param	dIntensity	- Diffuse intensity
*  @param	xPos		- The light position in the x-axis
*  @param	yPos		- The light position in the y-axis
*  @param	zPos		- The light position in the z-axis
*  @param	con			- The lights constant
*  @param	lin			- The lights linear
*  @param	exp			- The lights exponent
*
*/
PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat con, GLfloat lin, GLfloat exp)
	: Light(red, green, blue, aIntensity, dIntensity) {

	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

/**
*  This function binds passed parameters to the shader.
*
*  @param	ambientIntensityLocation - The ambient intensity location in the shader
*  @param	ambientColourLocation	 - The ambient color location in the shader
*  @param	diffuseIntensityLocation - The diffuse intensity location in the shader
*  @param	positionLocation		 - The position location in the shader
*  @param	constantLocation		 - The constant location in the shader
*  @param	linearLocation			 - The linear location in the shader
*  @param	exponentLocation		 - The exponent location in the shader
* 
*/
void PointLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation) {

	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

/**
*  Destructor of the PointLight.
*
*/
PointLight::~PointLight() {

}