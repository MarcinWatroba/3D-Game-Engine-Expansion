#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 colour;

out vec2 TexCoord;
out vec3 Colour;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	gl_Position = projection * model * vec4(vertex, 1.f);
	TexCoord = uv;
	Colour = colour;
}