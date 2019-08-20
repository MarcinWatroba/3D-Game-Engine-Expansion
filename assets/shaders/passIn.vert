#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normals;

out vec2 TexCoord;
out vec3 Normal;
out vec3 VertPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal_Matrix;
uniform mat4 shadowViewMat;

void main()
{
	gl_Position = projection * view * model * vec4(vertex, 1.f);
	TexCoord = uv;
	Normal = mat3(transpose(inverse(model))) * normals;
	VertPos = vec3(model * vec4(vertex, 1.f));
}