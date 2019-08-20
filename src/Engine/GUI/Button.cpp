#include <Engine\GUI\Button.h>
#include <Engine\GUI\Text.h>
#include <Engine\Component\Transform_2D.h>
#include <Engine\Component\RenderComp_2D.h>
#include <Engine\Mesh\Mesh_2D.h>
#include <Engine\Creators\Texture.h>
#include <glad\glad.h>
#include <Engine\GUI\Font.h>

Button::Button(const Button & button) : GameObject_2D(button)
{
	p_Text = button.p_Text;
	p_Font = button.p_Font;
}

Button::Button(int i_ObjectID_In, std::string s_Text_In, Font* p_Font_In, Mesh* p_Mesh_In, Texture* p_Texture_In, glm::vec2 v2_Position_In, glm::vec2 v2_Size_In, float f_Angle_In, float f_Layer_In, bool b_Render, std::string s_ObjectType_In)
{
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
	set_ObjectID(i_ObjectID_In);
	set_ObjectType(s_ObjectType_In);
}

void Button::update()
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update();
	p_Text->update();
}

void Button::render(Shader * p_Shader_In)
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update_Shader(p_Shader_In);
	if (b_RenderStatus) static_cast<RenderComp_2D*>(mipo_Components.find("RenderComp_2D")->second)->render(GL_TEXTURE_2D, GL_TRIANGLES, p_Shader_In);
	p_Text->set_RenderStatus(get_RenderStatus());
	p_Text->render(p_Shader_In);
}

void Button::set_Text(std::string s_Text_In)
{
	p_Text = new Text(get_ObjectID(), s_Text_In, get_Position() + get_Size() / 2.f, get_Angle(), p_Font, get_Layer() + 0.001f, get_RenderStatus(), get_ObjectType());
	p_Text->change_Origin(glm::vec2(p_Text->get_Size() / 2.f)); // Some tweaking to make it look nice
	p_Text->set_Position(get_Position() + get_Size() / 2.f);
	p_Text->set_Text(s_Text_In);
	p_Text->change_Origin(glm::vec2(p_Text->get_Size() / 2.f)); // Some tweaking to make it look nice
}


