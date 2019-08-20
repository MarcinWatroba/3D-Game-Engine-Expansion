#pragma once

#include <Engine/Mesh/Mesh.h>

class Mesh_3D : public Mesh
{
private:
	unsigned int ui_VBO[Buffer::Num];
	glm::vec3 minVert;
	glm::vec3 maxVert;
public:
	Mesh_3D() {};
	Mesh_3D(const char* pc_FileName_In, int i_DrawMode_In);
	glm::vec3 getMinVert()
	{
		return minVert;
	}
	glm::vec3 getMaxVert()
	{
		return maxVert;
	}
	Mesh_3D(const char* pc_FileName_In, int i_DrawMode_In, std::string s_ID_In);
	std::string get_Type();
	unsigned int get_SizeOfIndices();
};