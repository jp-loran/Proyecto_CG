#version 330 core
#define num_luces 2

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

uniform vec3 lightPosition_0;
uniform vec3 lightDirection_0;
uniform vec3 lightPosition_1;
uniform vec3 lightDirection_1;

uniform vec3 eye;
uniform float tiempo;

out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace[num_luces];
out vec3 Normal_cameraspace;

void main()
{

    vec4 PosL = vec4(aPos, 1.0f);
    
    PosL.y += 0.1f*PosL.x*sin(25.0f*(PosL.x));

    gl_Position = projection * view * model * PosL;

    TexCoords = aTexCoords;  

    vec3 vertexPosition_cameraspace = ( view * model * vec4(aPos,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
    
    vec3 lightPosition[2];
    lightPosition[0]=lightPosition_0;
    lightPosition[1]=lightPosition_1;
	vec3 lightDirection[2];
	lightDirection[0]=lightPosition_0;
	lightDirection[1]=lightPosition_1;

    for(int i=0; i < num_luces; i++){
    	vec3 LightPosition_cameraspace = ( view * vec4(lightPosition[i],1)).xyz;
    	LightDirection_cameraspace[i] = LightPosition_cameraspace + EyeDirection_cameraspace;
    }

    Normal_cameraspace = ( view * model * vec4(aNormal,0)).xyz;

    ex_N = aNormal;
}