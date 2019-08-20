#pragma once

#include <Engine\Mesh\Mesh_2D.h>

class Glyph : public Mesh_2D
{
private:
	char c_ID;
	int i_PushLetter;
	glm::vec2 v2_Offset;
	glm::vec2 v2_Length;
public:
	Glyph(glm::vec2 v2_Position_In, glm::vec2 v2_TextureSize_In, glm::vec2 v2_Length_In, glm::vec3 rgb_Colour_In, char c_ID_In, glm::vec2 v2_Offest_In, int i_PushLetter_In);
	
	char get_ID();
	int get_PushBy();
	glm::vec2 get_Offset();
	glm::vec2 get_Length();
};