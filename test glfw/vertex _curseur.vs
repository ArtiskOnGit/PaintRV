#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 VertexColor;

uniform int xcurseur;
uniform int ycurseur;

out vec4 FragColor;  
void main()
{
   gl_Position = vec4(aPos.x + xcurseur, aPos.y + ycurseur, 0.0, 1.0);
   FragColor = VertexColor;
};