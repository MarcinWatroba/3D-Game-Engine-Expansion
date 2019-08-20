#pragma once

#include <Engine\Game_Objects\GameObject_2D.h>
#include <vector>

class Shader;
class Font;
class Letter;

class Text : public GameObject_2D
{
private:
	std::vector<Letter*> vp_Letters;
	float f_Size;
	Font* p_Font;
	std::string s_Text;
	std::vector<glm::vec2> v2_OriginalPos;

public:
	Text() {};
	Text(const Text &text);
	Text(int i_ObjectID, std::string s_Text_In, glm::vec2 v2_Position_In, float f_Angle_In, Font* p_Font_In, float f_Layer_In, bool b_Render, std::string s_ObjectType_In);

	void set_Text(std::string s_Text_In);
	void update();
	void render(Shader* p_Shader_In);
	void change_Origin(glm::vec2 v2_Origin_In);
	void add_Letter(std::string s_Letter_In);
	void erase_Letter();
	bool is_Empty();
	glm::vec2 get_SizeOfFirstLetter();

	std::string get_Content();
};