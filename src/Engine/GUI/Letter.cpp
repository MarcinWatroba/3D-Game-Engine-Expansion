#include <Engine\GUI\Letter.h>
#include <Engine\GUI\Font.h>
#include <Engine\GUI\Glyph.h>
#include <Engine\Component\Transform_2D.h>
#include <Engine\Component\RenderComp_2D.h>
#include <Engine\Creators\Shader.h>
#include <glad\glad.h>

Letter::Letter(int i_ObjectID_In, char c_Letter_In, Font* p_Font_In, glm::vec2 v2_Position_In, float f_Angle, float f_Layer_In, bool b_Render, std::string s_ObjectType_In)
{
	p_Glyph = p_Font_In->get_Letter(c_Letter_In);

	std::string s_Name = "" + c_Letter_In;
	set_Name(s_Name);
	add_Component("Mesh_2D", p_Glyph);
	add_Component("Transform_2D", new Transform_2D());
	add_Component("RenderComp_2D", new RenderComp_2D());
	add_Texture("Texture_Atlas", p_Font_In->get_Texture());
	set_Position(glm::vec2(v2_Position_In.x + p_Glyph->get_Offset().x, v2_Position_In.y + p_Glyph->get_Offset().y));
	set_Origin(glm::vec2(0.f, 0.f));
	set_Angle(f_Angle);
	set_Layer(f_Layer_In);
	set_Size(p_Glyph->get_Length());
	set_Tiles(glm::vec2(1.f, 1.f));
	set_Tag("GUI");
	set_RenderStatus(b_Render);
	set_ObjectType(s_ObjectType_In);
	set_ObjectID(i_ObjectID_In);
}

int Letter::get_PushBy()
{
	return p_Glyph->get_PushBy();
}

glm::vec2 Letter::get_Offset()
{
	return p_Glyph->get_Offset();
}