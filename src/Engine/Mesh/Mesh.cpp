#include <Engine/Mesh/Mesh.h>

std::string Mesh::get_Type()
{
	return "Mesh";
}
unsigned int Mesh::get_VAO()
{
	//std::cout << "TEST" << std::endl;
	return ui_VAO;
}

unsigned int Mesh::get_VAO2()
{
	return VAO2;
}

unsigned int Mesh::get_SizeOfIndices()
{
	return vui_Indices.size();
}

std::string Mesh::get_ID()
{
	return s_ID;
}

unsigned int Mesh::get_SizeOfVertices()
{
	return vf_Vertices.size();
}

unsigned int Mesh::get_InstanceBufferHandle()
{
	//std::cout << "TEST" << std::endl;
	return instanceBufferHandle;
}
