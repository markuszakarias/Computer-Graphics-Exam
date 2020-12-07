#version 430

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

// const int's for the max amount of lights. Might remove later
const int MAX_POINT_LIGHTS = 15;
const int MAX_SPOT_LIGHTS = 3;

// Structs are created for each light.
struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

// The camera position
uniform vec3 eyePosition;

//Calculates the light by direction.
vec4 CalcLightByDirection(Light light, vec3 direction)
{
	// We multiply our color to our to the ambient intensity. This is from our light base class.
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;
	
	// The result of the angle of the light - will return a value between zero and one.
	// We use the dot product of a normalized Normal and direction.
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);

	// Much like with the ambientColour
	vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);

	// Set to zero, like we have no specular color
	vec4 specularColour = vec4(0, 0, 0, 0); 
	
	if(diffuseFactor > 0.0f)
	{
		// We can a vertex between these two position - normalized. We only want the direction.
		vec3 fragToEye = normalize(eyePosition - FragPos);

		// Were the reflection is going.
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		
		// This is why we needed the two values above normalized.
		float specularFactor = dot(fragToEye, reflectedVertex);

		// If we have specular lighting to apply.
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	// Return the calculated value.
	return (ambientColour + diffuseColour + specularColour);
}

// Uses the function above to return the calculation for the DirectionalLight.
vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

// Calculate our PointLights
vec4 CalcPointLight(PointLight pLight)
{
	// The position of the fragment, and subtract the light's position.
	vec3 direction = FragPos - pLight.position;

	// We need the distance, so it's the length of the direction.
	float distance = length(direction);

	// We also need to normalize this to get the actual direction. 
	direction = normalize(direction);
	
	// This would get the color as if it was a directional light.
	vec4 colour = CalcLightByDirection(pLight.base, direction);

	// Based on how far away the light is from the source, we need to change how much it is actually lit.
	// This works like a quadratic equation. 
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance +
						pLight.constant;
	
	return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	// Decides whether this area should be lit or not. Normalized version of the direction in the PointLight calc.
	vec3 rayDirection = normalize(FragPos - sLight.base.position);

	// We need this float to know whether or not to light the fragment in question.
	float slFactor = dot(rayDirection, sLight.direction);
	
	// If the spotlight factor is larger than the edge of the light. 
	if(slFactor > sLight.edge)
	{
		// Adds the color using the calc pointlight function recursively. 
		vec4 colour = CalcPointLight(sLight.base);
		
		// We need this calculation to avoid having very "hard" edges on the spotlight.
		return colour * (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - sLight.edge)));
		
	} else {
		return vec4(0, 0, 0, 0);
	}
}

// Go through the amount of PointLights and applies the appropriate function above.
vec4 CalcPointLights()
{
	// Reset the totalcolor.
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{		
		totalColour += CalcPointLight(pointLights[i]);
	}
	
	return totalColour;
}

// Go through the amount of SpotLights and applies the appropriate function above.
vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{		
		totalColour += CalcSpotLight(spotLights[i]);
	}
	
	return totalColour;
}
// Everything gets added together in main so we can output the final color. 
void main()
{
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();
	
	colour = texture(theTexture, TexCoord) * finalColour;
}