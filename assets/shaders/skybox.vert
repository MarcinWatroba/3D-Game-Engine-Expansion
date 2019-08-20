#version 330
layout (location = 0) in vec3 vertex;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
mat4 viewMat;

void main()
{

	viewMat = mat4(mat3(view));  
    TexCoords = vertex;

	gl_Position = projection * viewMat * model * vec4(vertex, 1.f);
}  