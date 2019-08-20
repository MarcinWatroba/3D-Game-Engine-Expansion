#include <Engine\GUI\Textbox.h>
#include <Engine\GUI\Text.h>
#include <Engine\Component\Transform_2D.h>
#include <Engine\Component\RenderComp_2D.h>
#include <Engine\Mesh\Mesh_2D.h>
#include <Engine\Creators\Texture.h>
#include <glad\glad.h>
#include <Engine\GUI\Font.h>

Textbox::Textbox(const Textbox & textbox) : GameObject_2D(textbox)
{
	p_Font = textbox.p_Font;
}

Textbox::Textbox(int i_ObjectID_In, Font * p_Font_In, Mesh * p_Mesh_In, Texture * p_Texture_In, glm::vec2 v2_Position_In, glm::vec2 v2_Size_In, float f_Angle_In, float f_Layer_In, bool b_Render, std::string s_ObjectType_In)
{
	s_Text = "";
	add_Component("Mesh_2D", p_Mesh_In);
	add_Component("Transform_2D", new Transform_2D());
	add_Component("RenderComp_2D", new RenderComp_2D());
	add_Texture("Texture_Atlas", p_Texture_In);
	set_Position(v2_Position_In);
	set_Origin(glm::vec2(0.f, 0.f));
	set_Size(v2_Size_In);
	set_Angle(f_Angle_In);
	p_Font = p_Font_In;
	set_Layer(f_Layer_In);
	set_Tiles(glm::vec2(1.f, 1.f));
	set_Tag("GUI");
	set_RenderStatus(b_Render);
	set_ObjectType(s_ObjectType_In);
	set_ObjectID(i_ObjectID_In);
	
	
}

void Textbox::add_Letter(std::string s_Text_In)
{
	if (vp_Lines.empty()) vp_Lines.push_back(new Text(get_ObjectID(), "", get_Position(), get_Angle(), p_Font, get_Layer() + 0.001f, get_RenderStatus(), get_ObjectType()));
	s_Text += s_Text_In;
	vp_Lines.back()->add_Letter(s_Text_In);
}

void Textbox::set_Text(std::string s_Text_In)
{
	if (vp_Lines.empty()) vp_Lines.push_back(new Text(get_ObjectID(), "", get_Position(), get_Angle(), p_Font, get_Layer() + 0.001f, get_RenderStatus(), get_ObjectType()));
	s_Text = s_Text_In;
	vp_Lines.back()->set_Text(s_Text);
}

void Textbox::erase_Letter()
{
	vp_Lines.back()->erase_Letter();
	s_Text.pop_back();
}

void Textbox::insert_NewLine()
{
	glm::vec2 v2_FirstLetterSize = vp_Lines.at(vp_Lines.size() - 1)->get_SizeOfFirstLetter();
	glm::vec2 v2_FirstLetterPos = vp_Lines.at(vp_Lines.size() - 1)->get_Position();
	v2_FirstLetterPos.y += v2_FirstLetterSize.y + 5.f;

	vp_Lines.push_back(new Text(get_ObjectID(), "", v2_FirstLetterPos, get_Angle(), p_Font, get_Layer() + 0.001f, get_RenderStatus(), get_ObjectType()));
}

void Textbox::delete_Line()
{
	if (vp_Lines.size() > 1 && vp_Lines.back()->is_Empty())
	{
		vp_Lines.back()->clean_Up();
		delete vp_Lines.back();
		vp_Lines.pop_back();
	}
}

bool Textbox::is_LineEmpty()
{
	return vp_Lines.back()->is_Empty();
}

std::string Textbox::get_Text()
{
	return s_Text;
}

void Textbox::update()
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update();
	for (int i = 0; i < vp_Lines.size(); i++) vp_Lines.at(i)->update();
}

void Textbox::render(Shader * p_Shader_In)
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update_Shader(p_Shader_In);
	if (b_RenderStatus) static_cast<RenderComp_2D*>(mipo_Components.find("RenderComp_2D")->second)->render(GL_TEXTURE_2D, GL_TRIANGLES, p_Shader_In);
	for (int i = 0; i < vp_Lines.size(); i++)
	{
		vp_Lines.at(i)->set_RenderStatus(get_RenderStatus());
		vp_Lines.at(i)->render(p_Shader_In);
	}	
}

