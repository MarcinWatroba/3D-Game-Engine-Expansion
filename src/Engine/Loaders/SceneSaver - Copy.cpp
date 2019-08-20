#include <Engine\Loaders\SceneSaver.h>
#include <TinyXML2\tinyxml2.h>
#include <iostream>
#include <vector>
#include <Engine\Game_Objects\GameObject_3D.h>
#include <Engine\Lighting\Point_Light.h>
#include <Engine\Component\RenderComp_3D.h>
#include <Engine\Creators\Texture.h>
#include <glm/gtx/quaternion.hpp>
#include <Engine\Mesh\Mesh_3D.h>

using namespace tinyxml2;

void SceneSaver::save(std::map<std::string, Game_Object*>& mspo_GameObjects_In, std::string s_SceneName_In)
{
	XMLDocument xmlDoc;
	XMLNode * pRoot = xmlDoc.NewElement("objects");
	xmlDoc.InsertFirstChild(pRoot);
	XMLElement * pElement;
	std::vector<Game_Object*> vo_Containers;

	for (auto const& pair : mspo_GameObjects_In)
	{
		if (pair.second->get_Tag() == "Object" && pair.second->get_Children().empty())
		{
			auto object_3D = static_cast<GameObject_3D*>(pair.second);
			auto mesh = static_cast<Mesh_3D*>(pair.second->get_Components().find("Mesh_3D")->second);

			pElement = xmlDoc.NewElement("new_Object3D");
			pElement->SetAttribute("name", object_3D->get_Name().c_str());
			pElement->SetAttribute("container", "No");
			pElement->SetAttribute("tag", object_3D->get_Tag().c_str());
			pElement->SetAttribute("mesh_ID", mesh->get_ID().c_str());
		
			std::string s_Components = "(";
			bool b_FirstTime = true;
			for (auto const& pair : object_3D->get_Components())
			{
				if (!b_FirstTime) s_Components += ", ";
				s_Components += pair.second->get_Type();
				b_FirstTime = false;
			}
			s_Components += ")";

			pElement->SetAttribute("component", s_Components.c_str());

			auto render_Comp = static_cast<RenderComp_3D*>(object_3D->get_Components().find("RenderComp_3D")->second);
			pElement->SetAttribute("diffuse_ID", render_Comp->get_Texture("Diffuse_Map")->get_ID().c_str());
			pElement->SetAttribute("specular_ID", render_Comp->get_Texture("Specular_Map")->get_ID().c_str());
			pElement->SetAttribute("shininess", render_Comp->get_Shininess());
			std::string s_Tiling = "(" + std::to_string(render_Comp->get_Tiles().x) + ", " + std::to_string(render_Comp->get_Tiles().y) + ")";
			pElement->SetAttribute("texture_Tiling", s_Tiling.c_str());

			std::string s_Origin = "(" + std::to_string(object_3D->get_Origin().x) + ", " + std::to_string(object_3D->get_Origin().y) + ", " + std::to_string(object_3D->get_Origin().z) + ")";
			pElement->SetAttribute("origin", s_Origin.c_str());
			std::string s_Position = "(" + std::to_string(object_3D->get_Position().x) + ", " + std::to_string(object_3D->get_Position().y) + ", " + std::to_string(object_3D->get_Position().z) + ")";
			pElement->SetAttribute("position", s_Position.c_str());
			std::string s_Orientation = "(" + std::to_string(object_3D->get_Rotation().w) + ", " + std::to_string(object_3D->get_Rotation().x) + ", " + std::to_string(object_3D->get_Rotation().y) + ", " + std::to_string(object_3D->get_Rotation().z) + ")";
			pElement->SetAttribute("orientation", s_Orientation.c_str());
			std::string s_Scale = "(" + std::to_string(object_3D->get_Scale().x) + ", " + std::to_string(object_3D->get_Scale().y) + ", " + std::to_string(object_3D->get_Scale().z) + ")";
			pElement->SetAttribute("scale", s_Scale.c_str());
			pRoot->InsertEndChild(pElement);	
		}
		else if (pair.second->get_Tag() == "Arrow" || pair.second->get_Tag() == "Light")
		{
			//Do nothing
		}
		else
		{
			vo_Containers.push_back(pair.second);
		}
	}
	
	for (int i = 0; i < vo_Containers.size(); i++)
	{
		auto object_3D = static_cast<GameObject_3D*>(vo_Containers.at(i));
		pElement = xmlDoc.NewElement("new_Object3D");
		pElement->SetAttribute("name", object_3D->get_Name().c_str());
		pElement->SetAttribute("container", "Yes");
		pElement->SetAttribute("tag", object_3D->get_Tag().c_str());
		
		std::string s_Components = "(";
		bool b_FirstTime = true;
		for (auto const& pair : object_3D->get_Components())
		{
			if (!b_FirstTime) s_Components += ", ";
			s_Components += pair.second->get_Type();
			b_FirstTime = false;
		}
		s_Components += ")";
		pElement->SetAttribute("component", s_Components.c_str());

		std::string s_Origin = "(" + std::to_string(object_3D->get_Origin().x) + ", " + std::to_string(object_3D->get_Origin().y) + ", " + std::to_string(object_3D->get_Origin().z) + ")";
		pElement->SetAttribute("origin", s_Origin.c_str());
		std::string s_Position = "(" + std::to_string(object_3D->get_Position().x) + ", " + std::to_string(object_3D->get_Position().y) + ", " + std::to_string(object_3D->get_Position().z) + ")";
		pElement->SetAttribute("position", s_Position.c_str());
		std::string s_Orientation = "(" + std::to_string(object_3D->get_Rotation().w) + ", " + std::to_string(object_3D->get_Rotation().x) + ", " + std::to_string(object_3D->get_Rotation().y) + ", " + std::to_string(object_3D->get_Rotation().z) + ")";
		pElement->SetAttribute("orientation", s_Orientation.c_str());
		std::string s_Scale = "(" + std::to_string(object_3D->get_Scale().x) + ", " + std::to_string(object_3D->get_Scale().y) + ", " + std::to_string(object_3D->get_Scale().z) + ")";
		pElement->SetAttribute("scale", s_Scale.c_str());

		std::string s_Children = "(";
		b_FirstTime = true;
		for (auto const& pair : vo_Containers.at(i)->get_Children())
		{
			if (!b_FirstTime) s_Children += ", ";
			s_Children += pair.second->get_Name();
			b_FirstTime = false;
		}
		s_Children += ")";
		pElement->SetAttribute("children", s_Children.c_str());
		pRoot->InsertEndChild(pElement);
	}
	xmlDoc.SaveFile("SavedData.xml");

	pRoot = xmlDoc.NewElement("lights");
	xmlDoc.InsertFirstChild(pRoot);
	
	for (auto const& pair : mspo_GameObjects_In)
	{
		if (pair.second->get_Tag() == "Light")
		{
			auto light = static_cast<Light*>(pair.second);

			pElement = xmlDoc.NewElement("new_Light");
			pElement->SetAttribute("type", light->get_Type().c_str());
			pElement->SetAttribute("tag", light->get_Tag().c_str());

			if (light->get_Type() == "Point_Light")
			{
				auto point_Light = static_cast<Point_Light*>(pair.second);

				pElement->SetAttribute("light_ID", point_Light->get_ID());
				std::string s_Position = "(" + std::to_string(point_Light->get_Position().x) + ", " + std::to_string(point_Light->get_Position().y) + ", " + std::to_string(point_Light->get_Position().z) + ")";
				pElement->SetAttribute("position", s_Position.c_str());

				std::string s_Ambient = "(" + std::to_string(point_Light->get_Ambient().x) + ", " + std::to_string(point_Light->get_Ambient().y) + ", " + std::to_string(point_Light->get_Ambient().z) + ")";
				pElement->SetAttribute("ambient", s_Ambient.c_str());

				std::string s_Diffuse = "(" + std::to_string(point_Light->get_Diffuse().x) + ", " + std::to_string(point_Light->get_Diffuse().y) + ", " + std::to_string(point_Light->get_Diffuse().z) + ")";
				pElement->SetAttribute("diffuse", s_Diffuse.c_str());

				std::string s_Specular = "(" + std::to_string(point_Light->get_Specular().x) + ", " + std::to_string(point_Light->get_Specular().y) + ", " + std::to_string(point_Light->get_Specular().z) + ")";
				pElement->SetAttribute("specular", s_Specular.c_str());

				std::string s_Constant = std::to_string(point_Light->get_Constant());
				pElement->SetAttribute("constant", s_Constant.c_str());
				
				std::string s_Linear = std::to_string(point_Light->get_Linear());
				pElement->SetAttribute("linear", s_Linear.c_str());
				
				std::string s_Quadratic = std::to_string(point_Light->get_Quadratic());
				pElement->SetAttribute("quadratic", s_Quadratic.c_str());
			}
			pRoot->InsertEndChild(pElement);
		}
	}

	xmlDoc.SaveFile(s_SceneName_In.c_str());
}
