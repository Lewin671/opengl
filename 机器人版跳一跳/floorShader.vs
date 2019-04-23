#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 normal;
out vec3 pointPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	// ���㷨�߾����÷������任����������ϵ
	normal = normalize(mat3(transpose(inverse(model))) * vec3(0,1,0));
	// ����任����������ϵ
	pointPos = vec3(model * vec4(aPos,1.0));
}