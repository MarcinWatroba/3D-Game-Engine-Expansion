#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec2 uv;
layout (location=2) in vec3 offset;

uniform mat4 Ms;
uniform mat4 Vs;
uniform mat4 Ps;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3  vertColour;
out vec3 colour;

out vec2 TexCoord;

void main()
{
TexCoord = uv;
//vec3 pos = VertexPosition * (gl_InstanceID / 100.0f);
//Transform from local to world to camera to NDCs
gl_Position =  projection * view * model * vec4(VertexPosition + offset,1.0);
}