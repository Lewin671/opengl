#version 330 core
layout (location = 2) in vec3 aPos;
layout (location = 3) in vec3 aColor;
layout(location=4) in vec3 aNormal;

out vec3 color;
out vec3 normal;
out vec3 pointPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	color = aColor;
	// ���㷨�߾����÷������任����������ϵ
	normal = normalize(mat3(transpose(inverse(model))) * aNormal);
	// ����任����������ϵ
	pointPos = vec3(model * vec4(aPos,1.0));
}