#version 330 core
layout (location = 5) in vec3 aPos;

uniform mat4 matrix;

void main()
{
	//vec3 p =  aPos;
	gl_Position = matrix * vec4(aPos,1);
}