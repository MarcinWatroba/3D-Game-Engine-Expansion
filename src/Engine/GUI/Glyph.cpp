#include <Engine\GUI\Glyph.h>
#include <Engine\Mesh\Mesh_2D.h>

Glyph::Glyph(glm::vec2 v2_Position_In, glm::vec2 v2_TextureSize_In, glm::vec2 v2_Length_In, glm::vec3 rgb_Colour_In, char c_ID_In, glm::vec2 v2_Offest_In, int i_PushLetter_In) : Mesh_2D(v2_Position_In, v2_TextureSize_In, v2_Length_In, rgb_Colour_In)
{
	c_ID = c_ID_In;
	v2_Offset = v2_Offest_In;
	i_PushLetter = i_PushLetter_In;
	v2_Length = v2_Length_In;
}

char Glyph::get_ID()
{
	return c_ID;
}

int Glyph::get_PushBy()
{
	return i_PushLetter;
}

glm::vec2 Glyph::get_Offset()
{
	return v2_Offset;
}

glm::vec2 Glyph::get_Length()
{
	return v2_Length;
}
