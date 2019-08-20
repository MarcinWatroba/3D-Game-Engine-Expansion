#include <Engine\Game_Objects\GameObject_2D.h>
#include <Engine\Component\RenderComp_2D.h>
#include <Engine\Component\Transform_2D.h>
#include <Engine\Mesh\Mesh_2D.h>
#include <iostream>
#include <glad\glad.h>

GameObject_2D::GameObject_2D(const GameObject_2D & p_NewObject_In) : Game_Object(p_NewObject_In)
{
	for (auto const& pair : p_NewObject_In.mipo_Components)
	{
		if (pair.second->get_Type() == "Mesh_2D")
		{
			auto mesh_2D = static_cast<Mesh_2D*>(pair.second);
			add_Component("Mesh_2D", mesh_2D);
		}
		else if (pair.second->get_Type() == "Transform_2D")
		{
			auto transform_2D = static_cast<Transform_2D*>(pair.second);
			add_Component("Transform_2D", new Transform_2D(*transform_2D));
		}
		else if (pair.second->get_Type() == "RenderComp_2D")
		{
			auto renderComp_2D = static_cast<RenderComp_2D*>(pair.second);
			add_Component("RenderComp_2D", new RenderComp_2D(*renderComp_2D));
		}
	}

	s_PrefabName = p_NewObject_In.s_PrefabName;
	s_Tag = p_NewObject_In.s_Tag;
}

void GameObject_2D::renderDepth(Shader* p_Shader_In)
{

}

glm::mat4 GameObject_2D::get_ParentMatrix()
{
	return static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->get_ModelMatrix();
}

void GameObject_2D::add_Component(std::string s_Name_In, Component * p_Component_In)
{
	mipo_Components.insert(std::pair<std::string, Component*>(s_Name_In, p_Component_In));

	if (s_Name_In == "RenderComp_2D")
	{
		auto found_Mesh = mipo_Components.find("Mesh_2D");

		if (found_Mesh == mipo_Components.end()) std::cout << "Mesh not found" << "\n";
		else
		{
			auto found_Render = mipo_Components.find("RenderComp_2D")->second;
			static_cast<RenderComp_2D*>(found_Render)->set_Mesh(static_cast<Mesh_2D*>(found_Mesh->second));
		}

	}
}

void GameObject_2D::add_Texture(std::string s_Name_In, Texture * p_Texture_In)
{
	static_cast<RenderComp_2D*>(mipo_Components.find("RenderComp_2D")->second)->add_Texture(s_Name_In, p_Texture_In);
}

void GameObject_2D::set_Tiles(glm::vec2 v2_Tiles_In)
{
	static_cast<RenderComp_2D*>(mipo_Components.find("RenderComp_2D")->second)->set_Tiles(v2_Tiles_In);
}

void GameObject_2D::update()
{
	if (po_Parent != nullptr)
	{
		static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->force_Update(); // Update must be forced to update any children
		static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update(static_cast<GameObject_2D*>(po_Parent)->get_ParentMatrix());
	}
	else
	{
		static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update();
	}
}

void GameObject_2D::force_Update()
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->force_Update();
}

void GameObject_2D::render(Shader * p_Shader_In)
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update_Shader(p_Shader_In);
	if (b_RenderStatus) static_cast<RenderComp_2D*>(mipo_Components.find("RenderComp_2D")->second)->render(GL_TEXTURE_2D, GL_TRIANGLES, p_Shader_In);
}

void GameObject_2D::render(Shader * p_Shader_In, bool skybox)
{
}

void GameObject_2D::clean_Up()
{
	for (auto const& components : mipo_Components) if (components.first != "Mesh_2D") delete components.second;
	mipo_Components.clear();
}

void GameObject_2D::set_Position(glm::vec2 v2_Position_In)
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->set_Position(v2_Position_In);
}

void GameObject_2D::set_Origin(glm::vec2 v2_Origin_In)
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->set_Origin(v2_Origin_In);
}

void GameObject_2D::set_Size(glm::vec2 v2_Size_In)
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->set_Size(v2_Size_In);
}

void GameObject_2D::set_Layer(float f_Layer_In)
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->set_Layer(f_Layer_In);
}

void GameObject_2D::set_Angle(float f_Angle_In)
{
	return static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->set_Angle(f_Angle_In);
}

void GameObject_2D::set_Colour(glm::vec3 rgb_Colour_In)
{
	static_cast<RenderComp_2D*>(mipo_Components.find("RenderComp_2D")->second)->set_Colour(rgb_Colour_In);
}

glm::vec2 GameObject_2D::get_Position()
{
	return static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->get_Position();
}

glm::vec2 GameObject_2D::get_Origin()
{
	return static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->get_Origin();
}

glm::vec2 GameObject_2D::get_Size()
{
	return static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->get_Size();
}

float GameObject_2D::get_Layer()
{
	return static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->get_Layer();
}

float GameObject_2D::get_Angle()
{
	return static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->get_Angle();
}

std::string GameObject_2D::get_Type()
{
	return "GameObject_2D";
}
