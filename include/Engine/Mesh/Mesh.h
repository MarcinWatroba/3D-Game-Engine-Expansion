#pragma once
#include <vector>
#include <map>
#include <Engine\Component\Component.h>

struct Vertex
{
	float x, y, z;
};

struct Normal
{
	float x, y, z;
};

struct UV
{
	float u, v;
};

struct Face
{
	unsigned int vertex;
	unsigned int uv;
	unsigned int normal;
};

struct CompleteVertex
{
	Vertex vertex;
	UV uv;
	Normal normal;
};

namespace Buffer
{
	enum Buffer { Vertex, Normal, UV, Num };
}

struct VertexIndex
{
	//Variables
	unsigned int ui_VertexID;
	unsigned int ui_UvID;
	unsigned int ui_NormalID;

	//Constructor
	VertexIndex() {}
	VertexIndex(const unsigned int ui_VertexID_In, const unsigned int ui_UvID_In, const unsigned int ui_NormalID_In) : ui_VertexID(ui_VertexID_In), ui_UvID(ui_UvID_In), ui_NormalID(ui_NormalID_In) {}

	//Compare operator for the map
	bool operator<(const VertexIndex f) const { return memcmp((void*)this, (void*)&f, sizeof(VertexIndex)) > 0; };
};

class Mesh : public Component
{
protected:
	std::vector<Vertex> vf_Vertices;
	std::vector<Normal> vf_Normals;
	std::vector<UV> vf_UVs;
	std::vector<Face> vui_Faces;
	std::vector<CompleteVertex> vf_Data;
	std::vector<unsigned int> vui_Indices;

	std::vector<float> vf_Verticesf;
	std::vector<float> vf_Normalsf;
	std::vector<float> vf_UVsf;
	std::vector<unsigned int> vui_Facesf;
	std::map<VertexIndex, unsigned int> moui_Indices;
	unsigned int instanceBufferHandle;
	unsigned int ui_VAO;
	unsigned int ui_EBO;
	unsigned int ui_VAO2;
	unsigned int VAO2;
	std::string s_ID;
public:
	Mesh() {};

	unsigned int get_VAO();
	unsigned int get_VAO2();
	unsigned int get_SizeOfIndices();

	unsigned int get_InstanceBufferHandle();
	std::string get_ID();
	unsigned int get_SizeOfVertices();
	std::string get_Type();
};