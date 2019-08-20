#include <Engine\GUI\Text.h>
#include <Engine\Component\Transform_2D.h>
#include <Engine\Component\RenderComp_2D.h>
#include <Engine\GUI\Letter.h>
#include <Engine\GUI\Font.h>
#include <Engine\Creators\Shader.h>
#include <glad\glad.h>

Text::Text(const Text & text) : GameObject_2D(text)
{
	f_Size = text.f_Size;
	p_Font = text.p_Font;
}

Text::Text(int i_ObjectID, std::string s_Text_In, glm::vec2 v2_Position_In, float f_Angle_In, Font* p_Font_In, float f_Layer_In, bool b_Render, std::string s_ObjectType_In)
{
	add_Component("Transform_2D", new Transform_2D());
	set_Origin(glm::vec2(0.f, 0.f));
	set_Position(v2_Position_In);
	set_Angle(f_Angle_In);
	set_Tag("GUI");
	p_Font = p_Font_In;
	s_Text = s_Text_In;
	set_Layer(f_Layer_In);
	set_ObjectID(i_ObjectID);
	set_RenderStatus(b_Render);
	set_ObjectType(s_ObjectType_In);

	if (s_Text_In != "")
	{
		for (int i = 0; i < s_Text_In.size(); i++)
		{
			if (i == 0) vp_Letters.push_back(new Letter(get_ObjectID(), s_Text[i], p_Font, get_Position(), get_Angle(), get_Layer() + 0.00001f, get_RenderStatus(), get_ObjectType()));
			else
			{
				glm::vec2 v2_Position = glm::vec2(vp_Letters.at(i - 1)->get_Position() - vp_Letters.at(i - 1)->get_Offset());
				v2_Position.x += vp_Letters.at(i - 1)->get_PushBy();
				vp_Letters.push_back(new Letter(get_ObjectID(), s_Text[i], p_Font, v2_Position, get_Angle(), get_Layer() + 0.00001f, get_RenderStatus(), get_ObjectType()));
			}

			vp_Letters.at(i)->set_ObjectID(get_ObjectID());
		}

		glm::vec2 v2_TempSize;
		for (int i = 0; i < vp_Letters.size(); i++) v2_TempSize += vp_Letters.at(i)->get_Size();

		//Set the size of the overall text
		//Count x of all letters, but only count the Y of the first letter
		set_Size(glm::vec2(v2_TempSize.x, vp_Letters.at(0)->get_Size().y));
	}
	else
	{
		set_Size(glm::vec2(1.f, 1.f));
	}
}

void Text::set_Text(std::string s_Text_In)
{
	if (std::strncmp(s_Text_In.c_str(), s_Text.c_str(), 100) != 0)
	{
		for (int i = 0; i < vp_Letters.size(); i++)
		{
			vp_Letters.at(i)->clean_Up();
			delete vp_Letters.at(i);
		}
		vp_Letters.clear();

		s_Text = s_Text_In;

		for (int i = 0; i < s_Text.size(); i++)
		{
			if (i == 0) vp_Letters.push_back(new Letter(get_ObjectID(), s_Text[i], p_Font, get_Position(), get_Angle(), get_Layer() - 0.00001f, get_RenderStatus(), get_ObjectType()));
			else
			{
				glm::vec2 v2_Position = glm::vec2(vp_Letters.at(i - 1)->get_Position() - vp_Letters.at(i - 1)->get_Offset());
				v2_Position.x += vp_Letters.at(i - 1)->get_PushBy();
				vp_Letters.push_back(new Letter(get_ObjectID(), s_Text[i], p_Font, v2_Position, get_Angle(), get_Layer() - 0.00001f, get_RenderStatus(), get_ObjectType()));
			}

			vp_Letters.at(i)->set_ObjectID(get_ObjectID());
		}

		glm::vec2 v2_TempSize;
		for (int i = 0; i < vp_Letters.size(); i++) v2_TempSize += vp_Letters.at(i)->get_Size();

		//Set the size of the overall text
		//Count x of all letters, but only count the Y of the first letter
		set_Size(glm::vec2(v2_TempSize.x, vp_Letters.at(0)->get_Size().y));
	}
}

void Text::update()
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update();
	for (int i = 0; i < vp_Letters.size(); i++) vp_Letters.at(i)->update();
}

void Text::render(Shader* p_Shader_In)
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update_Shader(p_Shader_In);
	for (int i = 0; i < vp_Letters.size(); i++)
	{
		vp_Letters.at(i)->set_RenderStatus(get_RenderStatus());
		vp_Letters.at(i)->render(p_Shader_In);
	}
}

//Important! Since we have added new laters in the constructor using position variable
// This means that setting Text's origin will not change anything!
//In that case we are going to set the origin in this function and update all letters
void Text::change_Origin(glm::vec2 v2_Origin_In)
{
	if (get_Origin() != v2_Origin_In)
	{
		set_Origin(v2_Origin_In);
		for (int i = 0; i < vp_Letters.size(); i++) vp_Letters[i]->set_Position(vp_Letters[i]->get_Position() - get_Origin());
	}
}

void Text::add_Letter(std::string s_Letter_In)
{
	if (vp_Letters.size() == 0) vp_Letters.push_back(new Letter(get_ObjectID(), s_Letter_In[0], p_Font, get_Position(), get_Angle(), get_Layer(), get_RenderStatus(), get_ObjectType()));
	else
	{
		glm::vec2 v2_Position = vp_Letters.at(vp_Letters.size() - 1)->get_Position() - vp_Letters.at(vp_Letters.size() - 1)->get_Offset();
		v2_Position.x += vp_Letters.at(vp_Letters.size() - 1)->get_PushBy();
		vp_Letters.push_back(new Letter(get_ObjectID(), s_Letter_In[0], p_Font, v2_Position, get_Angle(), get_Layer(), get_RenderStatus(), get_ObjectType()));
	}
}

void Text::erase_Letter()
{
	if (vp_Letters.size() != 0)
	{
		vp_Letters.at(vp_Letters.size() - 1)->clean_Up();
		delete vp_Letters.at(vp_Letters.size() - 1);
		vp_Letters.pop_back();
	}
}

bool Text::is_Empty()
{
	return vp_Letters.empty();
}

glm::vec2 Text::get_SizeOfFirstLetter()
{
	return vp_Letters.at(0)->get_Size();
}

std::string Text::get_Content()
{
	return s_Text;
}
