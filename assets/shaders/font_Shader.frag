#version 330

in vec2 TexCoord;
in vec3 Colour;

out vec4 colour;

uniform sampler2D object_Texture;
uniform vec3 rgb_Colour;

void main()
{
	vec4 texColor = texture(object_Texture, vec2(TexCoord.x, TexCoord.y));

	texColor.r *= rgb_Colour.r;
	texColor.g *= rgb_Colour.g;
	texColor.b *= rgb_Colour.b;

	if(texColor.a < 0.8) discard;
		
	colour = texColor;
	

}