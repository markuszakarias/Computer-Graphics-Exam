#version 430

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out float Height;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	// Calculate the MVP matrix and applying it to the gl_Position.
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	TexCoord = tex;
	
	// Calculating normals based on height
	float hL = pos.y - 1.0;
	float hR = pos.y + 1.0;
	float hD = pos.y - 0.0;
	float hU = pos.y + 0.0;
	
	// Deducing terrain normals
	Normal = vec3(hL - hR, 2.0, hD - hU) * norm;
		
	//Normal = mat3(transpose(inverse(model))) * norm;

	// Our fragment position - We do something called swizzling.
	FragPos = (model * vec4(pos, 1.0)).xyz;

	// Sending height to fragment shader to calculate which colour goes where 
	Height = pos.y;
}