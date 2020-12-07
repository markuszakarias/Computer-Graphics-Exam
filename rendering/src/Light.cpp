#include "Light.h"

/**
*  The parent class which has functionality that the DirectionalLight,   
*  SpotLight and PointLight class inherits from. Could be an idea to     
*  have all the functions regarding the lights in here, but for now they are separate.		
*  
*  @name Light.cpp
*  @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*  Default constructor for the Light class.
*
*/
Light::Light() {

	colour = glm::vec3(1.0f, 1.0f, 1.0f);

	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

/**
*  Construct a new Light. Directional, Point and SpotLight derive from this constructor.
*
*  @param	red			- RGB red color
*  @param	green		- RGB green color
*  @param	blue		- RGB blue color
*  @param	aIntensity	- The ambient intensity of the light
*  @param	dIntensity	- The diffuse intensity of the light
* 
*/
Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) {

	colour = glm::vec3(red, green, blue);

	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

/**
*  Construct a new Light. Directional, Point and SpotLight derive from this constructor.
*
*  @param	shadowWidth	 - The shadowmap width
*  @param	shadowHeight - The shadowmap height
*  @param	red			 - RGB red color
*  @param	green		 - RGB green color
*  @param	blue		 - RGB blue color
*  @param	aIntensity	 - The ambient intensity of the light
*  @param	dIntensity	 - The diffuse intensity of the light
*
*/
Light::Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity) {

	colour = glm::vec3(red, green, blue);

	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;

	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

/**
*  This function binds passed parameters to the shader
*
*  @param	ambientIntensityLocation	- Bind the ambientIntensityLocation to the shader
*  @param	ambientColorLocation		- Bind the ambientColorLocation to the shader
*/
void Light::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation) {

	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
}

/**
*  Light destructor
*
*/
Light::~Light() {

}