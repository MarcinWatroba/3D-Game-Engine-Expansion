#pragma once

#include <Engine\Mesh\Mesh.h>


struct Vertex_2D
{
	float x, y, z;
	Vertex_2D() {};
	Vertex_2D(float f_X, float f_Y, float f_Z) { x = f_X; y = f_Y; z = f_Z; }
};

struct UV_2D
{
	float u, v;
	UV_2D() {};
	UV_2D(float f_U, float f_V) { u = f_U; v = f_V; }
};

struct Colour
{
	float r, g, b;
	Colour(float f_R, float f_G, float f_B) { r = f_R; g = f_G; b = f_B; }
};

struct Indice
{
	unsigned int first, second, third;
	Indice(unsigned int ui_First, unsigned int ui_Second, unsigned int ui_Third) { first = ui_First; second = ui_Second; third = ui_Third; }
};

namespace Buffer_2D
{
	enum Buffer { Vertex, UV, Colour, Num };
}

class Mesh_2D : public Mesh
{
private:
	std::vector<Vertex_2D> vf_Vertices2D;
	std::vector<UV_2D> vf_UVs2D;
	std::vector<Colour> vf_Colours;
	std::vector<Indice> vui_Indices2D;
	unsigned int ui_VBO[Buffer_2D::Num];
public:
	Mesh_2D(glm::vec2 v2_TexturePos_In, glm::vec2 v2_TextureSize_In, glm::vec2 v2_TextureLength_In, glm::vec3 rgb_Colour_In);
	std::string get_Type();
	unsigned int get_SizeOfIndices();
};