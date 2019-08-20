#version 430
layout (location = 0) in vec3 VertexPosition;

uniform vec3 lightPos;
uniform mat4 model;
uniform mat4 view;
void main()
{
	//mat4 models = model*view;
	//models = vec3(models * vec4(VertexPosition, 1.0));
    gl_Position = model * vec4(VertexPosition, 1.0);
}  