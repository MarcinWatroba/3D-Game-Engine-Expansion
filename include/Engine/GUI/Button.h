#pragma once

#include <Engine\Game_Objects\GameObject_2D.h>

class Text;
class Font;
class Mesh;
class Texture;

class Button : public GameObject_2D
{
private:
	Text* p_Text;
	Font* p_Font;

public:
	Button() {}
	Button(const Button &button);
	Button(int i_ObjectID_In, std::string s_Text_In, Font* p_Font_In, Mesh* p_Mesh_In, Texture* p_Texture_In, glm::vec2 v2_Position_In, glm::vec2 v2_Size_In, float f_Angle_In, float f_Layer_In, bool b_Render, std::string s_ObjectType_In);
	void update();
	void render(Shader* p_Shader_In);

	void set_Text(std::string s_Text_In);
};