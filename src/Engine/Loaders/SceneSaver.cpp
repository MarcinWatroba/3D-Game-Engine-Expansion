#include <Engine\Loaders\SceneSaver.h>
#include <TinyXML2\tinyxml2.h>
#include <iostream>
#include <vector>
#include <Engine\Game_Objects\GameObject_3D.h>
#include <Engine\Game_Objects\GameObject_Instanced.h>
#include <Engine\Lighting\Point_Light.h>
#include <Engine\Component\RenderComp_3D.h>
#include <Engine\Component\RenderComp_Instanced.h>
#include <Engine\Creators\Texture.h>
#include <glm/gtx/quaternion.hpp>
#include <Engine\Mesh\Mesh_3D.h>
#include <Engine\Mesh\Mesh_Instanced.h>

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
		if (pair.second->get_Tag() != "Particle" && pair.second->get_Tag() != "Light")
		{
			auto object_3D = static_cast<GameObject_3D*>(pair.second);

			pElement = xmlDoc.NewElement("new_Object3D");
			pElement->SetAttribute("name", object_3D->get_Name().c_str());
			pElement->SetAttribute("prefab", object_3D->get_PrefabName().c_str());

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
	}

	for (auto const& pair : mspo_GameObjects_In)
	{
		if (pair.second->get_Tag() == "Particle")
		{
			auto object_Instanced = static_cast<GameObject_Instanced*>(pair.second);

			pElement = xmlDoc.NewElement("new_ObjectParticle ");
			pElement->SetAttribute("name", object_Instanced->get_Name().c_str());
			pElement->SetAttribute("prefab", object_Instanced->get_PrefabName().c_str());

			std::string s_Origin = "(" + std::to_string(object_Instanced->get_Origin().x) + ", " + std::to_string(object_Instanced->get_Origin().y) + ", " + std::to_string(object_Instanced->get_Origin().z) + ")";
			pElement->SetAttribute("origin", s_Origin.c_str());
			std::string s_Position = "(" + std::to_string(object_Instanced->get_Position().x) + ", " + std::to_string(object_Instanced->get_Position().y) + ", " + std::to_string(object_Instanced->get_Position().z) + ")";
			pElement->SetAttribute("position", s_Position.c_str());
			std::string s_Orientation = "(" + std::to_string(object_Instanced->get_Rotation().w) + ", " + std::to_string(object_Instanced->get_Rotation().x) + ", " + std::to_string(object_Instanced->get_Rotation().y) + ", " + std::to_string(object_Instanced->get_Rotation().z) + ")";
			pElement->SetAttribute("orientation", s_Orientation.c_str());
			std::string s_Scale = "(" + std::to_string(object_Instanced->get_Scale().x) + ", " + std::to_string(object_Instanced->get_Scale().y) + ", " + std::to_string(object_Instanced->get_Scale().z) + ")";
			pElement->SetAttribute("scale", s_Scale.c_str());
			std::string s_MaxParticles = std::to_string(object_Instanced->getMax());
			pElement->SetAttribute("max_Particles", s_MaxParticles.c_str());
			std::string s_ParticleRange = "(" + std::to_string(object_Instanced->get_Range().x) + ", " + std::to_string(object_Instanced->get_Range().y) + ", " + std::to_string(object_Instanced->get_Range().z) + ")";
			pElement->SetAttribute("particle_Range", s_ParticleRange.c_str());
			std::string s_ParticleColour = "(" + std::to_string(object_Instanced->get_Colour().x) + ", " + std::to_string(object_Instanced->get_Colour().y) + ", " + std::to_string(object_Instanced->get_Colour().z) + ")";
			pElement->SetAttribute("particle_Colour", s_ParticleColour.c_str());
			std::string s_ParticleSpeed = "(" + std::to_string(object_Instanced->get_Particle_Speed().x) + ", " + std::to_string(object_Instanced->get_Particle_Speed().y) + ", " + std::to_string(object_Instanced->get_Particle_Speed().z) + ")";
			pElement->SetAttribute("particle_Speed", s_ParticleSpeed.c_str());
			std::string s_ParticleLife = std::to_string(object_Instanced->get_Life());
			pElement->SetAttribute("particle_Life", s_ParticleLife.c_str());
			pRoot->InsertEndChild(pElement);


		}
	}

	pRoot = xmlDoc.NewElement("lights");
	xmlDoc.InsertFirstChild(pRoot);
	for (auto const& pair : mspo_GameObjects_In)
	{
		if (pair.second->get_Tag() == "Light")
		{
			pElement = xmlDoc.NewElement("new_Light");

			if (((Light*)pair.second)->get_Type() == "Point_Light")
			{
				auto point_Light = static_cast<Point_Light*>(pair.second);
				pElement->SetAttribute("name", point_Light->get_Name().c_str());
				pElement->SetAttribute("prefab", point_Light->get_PrefabName().c_str());
				pElement->SetAttribute("type", "Point_Light");
				std::string s_Position = "(" + std::to_string(point_Light->get_Position().x) + ", " + std::to_string(point_Light->get_Position().y) + ", " + std::to_string(point_Light->get_Position().z) + ")";
				pElement->SetAttribute("position", s_Position.c_str());
				pRoot->InsertEndChild(pElement);
			}
		}
	}

	xmlDoc.SaveFile(s_SceneName_In.c_str());
}
