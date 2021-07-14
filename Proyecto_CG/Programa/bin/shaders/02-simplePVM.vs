#version 330 core
layout (location = 0) in vec3  inPos;
layout (location = 1) in vec4  inColor;

out vec4 vxColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    vec4 PosH = vec4(inPos, 1.0f);
    gl_Position = projection * view * model * PosH; 
    vxColor = inColor;
    
}