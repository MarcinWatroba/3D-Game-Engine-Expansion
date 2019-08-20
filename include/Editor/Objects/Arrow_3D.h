#pragma once
#include <Engine\Game_Objects\GameObject_3D.h>

class Mesh_3D;

class Arrow_3D : public GameObject_3D
{
public:
	Arrow_3D() {};
	Arrow_3D(int i_ID_In, std::string s_Name_In, Mesh_3D* p_Mesh_In, glm::quat v2_Rotation_In, Texture* p_Diffuse_In, Texture* p_Specular_In);
};