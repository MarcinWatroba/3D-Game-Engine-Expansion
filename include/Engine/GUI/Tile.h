#pragma once

#include <Engine\Game_Objects\GameObject_2D.h>

class Texture;
class Mesh;

class Tile : public GameObject_2D
{
public:
	Tile() {};
	Tile(const Tile &tile);
	Tile(int i_ObjectID_In, std::string s_Text_In, Mesh* p_Mesh_In, Texture* p_Texture_In, glm::vec2 v2_Position_In, glm::vec2 v2_Size_In, float f_Angle_In, float f_Layer_In);

	void update();
	void render(Shader* p_Shader_In);
};