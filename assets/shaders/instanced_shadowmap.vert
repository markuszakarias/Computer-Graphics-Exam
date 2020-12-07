#version 430

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in mat4 aInstanceMatrix;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 DirectionalLightSpacePos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;

void main()
{
    TexCoord = tex;

    // We use aInstance here instead of model because of instanced
    gl_Position = projection * view * aInstanceMatrix * vec4(pos, 1.0); 
    DirectionalLightSpacePos = directionalLightTransform * aInstanceMatrix * vec4(pos, 1.0);

    Normal = mat3(transpose(inverse(aInstanceMatrix))) * norm;

    FragPos = (aInstanceMatrix * vec4(pos, 1.0)).xyz;
}