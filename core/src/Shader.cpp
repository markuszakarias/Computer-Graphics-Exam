#include "Shader.h"

/**
*	The Shader class holds all of the information written in the fragment and vertex GLSL files. The files are
*	parsed and compiled. Each uniform from the shader files are located and passed to a GLuint uniform, which
*	is later used in the application. 
*
*  @name Shader.cpp
*  @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*	Default constructor for the Shader. Initializes the uniforms and other variables. 
*
*/
Shader::Shader() {

	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

/**
*	Creates a shader from file and uses other function to parse and compile the shader.
*
*   @param vertexLocation	-	Takes in a string to the vertex shader location.
*   @param fragmentLocation	-	Takes in a string to the fragment shader location.
* 
*	@see readFile(), compileShader()
*/
void Shader::createShaderFromFile(const char* vertexLocation, const char* fragmentLocation) {

	std::string vertexString = readFile(vertexLocation);
	std::string fragmentString = readFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode);
}

/**
*	Reads a file and parses the content.
*
*	@param fileLocation	- Takes in a pointer to char as the file location.
* 
*	@return the content of the file.
*/
std::string Shader::readFile(const char* fileLocation) {
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		std::cout << "Failed to read: " << fileLocation << " File doesn't exist." << "\n";
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

/**
*	Compiles the shader. Uses the addShader function to pass the shaderId and vertex/fragment shader
*	to the GL VERTEX/FRAGMENT SHADER. All the uniforms location are retrieved from the vertex and 
*	fragment shader, and passed to a GLuint uniform variable. Might have been a better idea to have 
*	separate compileShader functions for each shader, and only retrieve the uniforms for that shader,
*	but getting the uniform locations is generally cheap and to reduce the amount of code it was a 
*	sound solution for our application. 
*
*   @param vertexCode	-	Compiles the vertex code from the shader
*   @param fragmentCode -	Compiles the fragment code from the shader
*/
void Shader::compileShader(const char* vertexCode, const char* fragmentCode) {

	shaderID = glCreateProgram();

	if (!shaderID) {
		std:: cout << "Error creating shader program!\n";
		return;
	}

	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);


	// ERROR CHECKS
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {

		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error LINKING program: " << eLog;
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {

		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error VALIDATING program: " << eLog;
		return;
	}

	//All of the uniform variables are set to the values from the shader

	uniformView = glGetUniformLocation(shaderID, "view");
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformTex = glGetUniformLocation(shaderID, "theTexture");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
	uniformMinimapTexture = glGetUniformLocation(shaderID, "screenTexture");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	
	/* -- These for-loops are for the point- and spotlights. Since it's possible to create multiple        -- 
	   -- lights it made more sense to have a for loop to get the uniform variables, instead of having     -- 
	   -- to assign uniformPointLight.Something to object 1, 2, 3 and so on. It saves a lot of code.       -- 
	   --																							       -- 
	   -- The '\0' is the null terminator, which here means the end of the string. We use the snprintf     -- 
	   -- because this lets us print something to the buffer, here the localBuffer. And the %d represents  -- 
	   -- the [i] variable. The '3' should be a MAX variable like, MAX_POINT_LIGHTS.																					   -- 
	*/

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (unsigned int i = 0; i < 15; i++) {

		char localBuffer[100] = { '\0' };

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].base.colour", i);
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, localBuffer);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (unsigned int i = 0; i < 3; i++) {

		char localBuffer[100] = { '\0' };

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.base.colour", i);
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, localBuffer);

		snprintf(localBuffer, sizeof(localBuffer), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, localBuffer);
	}
}

/**
* 
*	PLACEHOLDER
* 
*/
void Shader::SetTexture(GLuint textureUnit) {

	glUniform1i(uniformTex, textureUnit);
}

/**
*
*	PLACEHOLDER
*
*/
void Shader::SetDirectionalShadowMap(GLuint textureUnit) {

	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

/**
*
*	PLACEHOLDER
*
*/
void Shader::SetDirectionalLightTransform(glm::mat4* lTransform) {

	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}


/**
*	These set the different lights during runtime. The uniform variables and structs are used here.
*	Sets the DirectionalLight in the scene, is used during runtime. Uses the useLight to pass in 
*	the uniform variables that we got from the shader in the functions above.
*
*	@param dLight	-	The DirectionalLight passed in.
* 
*	@see useLight()
*/
void Shader::setDirectionalLight(std::shared_ptr<DirectionalLight>& dLight) {

	dLight->useLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

/**
*	Sets the PointLights in the scene, is used during runtime. Uses the useLight to pass in
*	the uniform variables that we got from the shader in the functions above. Has a for loop
*	to go through the amount of point lights set in the scene.
*
*	@param pLight		-	The PointLight passed in.
*	@param lightCount	-	The amount of point lights passed in.
* 
*	@see useLight()
*/
void Shader::setPointLights(PointLight* pLight, unsigned int lightCount) {

	if (lightCount > 15) lightCount = 15;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {

		pLight[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}

/**
*	Sets the SpotLights in the scene, is used during runtime. Uses the useLight to pass in
*	the uniform variables that we got from the shader in the functions above. Has a for loop
*	to go through the amount of spot lights set in the scene.
*
*	@param sLight		-	The SpotLight passed in.
*	@param lightCount	-	The amount of spot lights passed in.
* 
*	@see useLight()
*/
void Shader::setSpotLights(SpotLight* sLight, unsigned int lightCount) {

	if (lightCount > 3) lightCount = 3;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {

		sLight[i].useLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge);
	}
}

/**
*	Uses the shader during runtime.
*
*/
void Shader::useShader() {

	glUseProgram(shaderID);
}

void Shader::unUseShader() {

	glUseProgram(0);
}

/**
*	Clears the shader.
*
*/
void Shader::clearShader() {

	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
}

/**
*	Creates, compiles and attaches the shader. Is used in the compileShader above.
*
*   @param theProgram	-	The shader program that gets attached.
*   @param shaderCode	-	The shader code from the file.
*   @param shaderType	-	A GLenum of the shader type.
*/
void Shader::addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {

	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the: " << shaderType << " error is: "<<  eLog;
		return;
	}

	glAttachShader(theProgram, theShader);
}

/**
*	Destructor for the Shader.
*
*	@see clearShader()
*/
Shader::~Shader() {

	clearShader();
}

/**
*	Calculates the average of normals. Takes in all of the vertices and indices of an object, and the size
*	of data for each vertex. It also takes in where the normals are in each vertex data. 
* 
*	To avoid that the calculation will overflow the original (smaller) type before conversion
*	to the result (larger) type we can either cast to double or use unsigned long int.
*
*   @param indices		-	The indices vector of the mesh.
*   @param indiceCount	-	The indices vector size of the mesh.
*   @param vertices		-	The vertices vector of the mesh.
*   @param verticeCount	-	The vertices vector size of the mesh.
*   @param vLength		-	The vertex data length, the amount of data in each vertex.
*   @param normalOffset	-	The offset to get to the normals of the mesh.
*/
void Shader::calculateAverageNormals(std::vector<GLuint> indices, unsigned int indiceCount, std::vector<GLfloat> vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset) {

	//We want to go through every triangle we have in the mesh and increment 3 at a time.
	//This is to jump to each triangle since 3 indices make up a triangle.
	for (size_t i = 0; i < indiceCount; i += 3) {

		//We need to multiply each indices element by vLength, It moves you down the "table" by 8.
		unsigned long int in0 = double(indices[i]) * vLength;
		unsigned long int in1 = double(indices[i + 1]) * vLength;
		unsigned long int in2 = double(indices[i + 2]) * vLength;

		//We create two vector and connect them, that we will use later to create a line.
		glm::vec3 v1(vertices[in1] - vertices[in0], 
					 vertices[in1 + 1] - vertices[in0 + 1], 
					 vertices[in1 + 2] - vertices[in0 + 2]);

		glm::vec3 v2(vertices[in2] - vertices[in0], 
					 vertices[in2 + 1] - vertices[in0 + 1], 
					 vertices[in2 + 2] - vertices[in0 + 2]);

		//Now we want to start adding these values to our normal. We add the normalOffset to work 
		//on the normal in the vertex data.
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		//Now we want to start adding these values to our normal. 
		//We add the normalOffset to work on the normal in the vertex data.
		in0 += normalOffset; 
		in1 += normalOffset; 
		in2 += normalOffset;

		vertices[in0] += normal.x; 
		vertices[in0 + 1] += normal.y; 
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x; 
		vertices[in1 + 1] += normal.y; 
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x; 
		vertices[in2 + 1] += normal.y; 
		vertices[in2 + 2] += normal.z;
	}

	//Then we want to normalize these values, to unit vector, much like above. nOffset will be the normal 
	//offset for each individual one. Every time we go through its counting up each of these 0, 1, 2 etc. 
	//and grabbing relevant one from the list of vertex and going to the offset section of it. Then we 
	//normalize this vector. Then we assign them to each vertex axis (x, y, z).
	for (size_t i = 0; i < verticeCount / vLength; i++) {

		unsigned long int nOffset = i * vLength + normalOffset;

		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);

		vec = glm::normalize(vec);

		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}