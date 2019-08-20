#version 330

in vec2 TexCoord;

out vec4 colour;

struct Material
{
	sampler2D diffuse;
};

uniform Material material;

void main()
{
	colour = texture(material.diffuse, TexCoord);
}