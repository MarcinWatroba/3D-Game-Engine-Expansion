#pragma once

#include <Engine/Mesh/Mesh.h>



class Mesh_Instanced : public Mesh
{
private:
	unsigned int ui_VBO;	
public:
	Mesh_Instanced() {};
	Mesh_Instanced(const char * pc_FileName_In, int i_DrawMode_In);

	std::string get_Type();

};