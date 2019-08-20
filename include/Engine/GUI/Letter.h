#pragma once

#include <Engine\Game_Objects\GameObject_2D.h>

class Font;
class Glyph;
class Shader;

class Letter : public GameObject_2D
{
private:
	Glyph* p_Glyph;
public:
	Letter() {};
	Letter(int i_ObjectID_In, char c_Letter_In, Font* p_Font_In, glm::vec2 v2_Position_In, float f_Angle, float f_Layer_In, bool b_Render, std::string s_ObjectType_In);

	int get_PushBy();
	glm::vec2 get_Offset();
};