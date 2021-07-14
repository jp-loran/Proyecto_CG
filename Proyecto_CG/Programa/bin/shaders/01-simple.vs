#version 330 core
layout (location = 0) in vec3  inPos;
layout (location = 1) in vec4  inColor;

out vec4 vxColor;

void main()
{

    vec4 PosH = vec4(inPos, 1.0f);
    gl_Position = PosH; 
    vxColor = inColor;
    
}