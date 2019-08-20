#version 330
in vec3 TexCoords;
out vec4 colour;

uniform samplerCube cubemap;

void main()
{    
    colour = texture(cubemap, TexCoords);
	//colour = vec4(1.0f);
}