#version 330 core
layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aNormal;
layout (location = 2) in vec2  aTexCoords;
layout (location = 3) in vec3  tangent;
layout (location = 4) in vec3  bitangent;

out vec2 TexCoords;
out vec3 ex_N;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPosition;
uniform vec3 lightDirection;

out vec3 EyeDirection_cameraspace;

void main()
{

    vec4 PosL = vec4(aPos, 1.0f);
    gl_Position = projection * view * model * PosL;

    TexCoords = aTexCoords;    

    vec3 vertexPosition_cameraspace = ( view * model * vec4(aPos, 1.0)).xyz;
    EyeDirection_cameraspace = lightPosition - vertexPosition_cameraspace;
    ex_N = aNormal;
}