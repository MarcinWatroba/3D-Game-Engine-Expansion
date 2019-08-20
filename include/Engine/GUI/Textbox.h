#pragma once

#include <Engine\Game_Objects\GameObject_2D.h>
#include <vector>

class Font;
class Mesh;
class Texture;
class Text;

class Textbox : public GameObject_2D
{
private:
	std::string s_Text;
	Font* p_Font;

	std::vector<Text*> vp_Lines;
public:
	Textbox() {};
	Textbox(const Textbox &textbox);
	Textbox(int i_ObjectID_In, Font* p_Font_In, Mesh* p_Mesh_In, Texture* p_Texture_In, glm::vec2 v2_Position_In, glm::vec2 v2_Size_In, float f_Angle_In, float f_Layer_In, bool b_Render, std::string s_ObjectType_In);

	void add_Letter(std::string s_Text_In);
	void set_Text(std::string s_Text_In);
	void erase_Letter();
	void insert_NewLine();
	void delete_Line();
	bool is_LineEmpty();
	std::string get_Text();



	void update();
	void render(Shader* p_Shader_In);
};