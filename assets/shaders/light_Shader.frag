#version 330

layout (location = 0) out vec4 colour;
layout (location = 1) out vec4 BrightColor;  

uniform vec3 light_Colour;

void main()
{
    float brightness = dot(light_Colour, vec3(0.2126, 0.7152, 0.0722));
	colour = vec4(light_Colour * brightness, 1.f);
}