#include <Engine\Loaders\PrefabLoader.h>
#include <iostream>
#include <Engine\Loaders\Loader.h>
#include <Engine\Game_Objects\GameObject_3D.h>
#include <Engine\Component\Transform_3D.h>
#include <Engine\Component\RenderComp_3D.h>
#include <Engine\Game_Objects\GameObject_Instanced.h>
#include <Engine\Component\Transform_Instanced.h>
#include <Engine\Component\RenderComp_Instanced.h>
#include <Engine\Lighting\Point_Light.h>
#include <Engine\Loaders\StatsLoader.h>
#include <Engine\Stats\Stats.h>
#include <Engine\Component\Character.h>
#include <Engine\Component\RigidBody.h>
#include <Engine\Component\BoxCollider_3D.h>
#include <Engine\Component\Respond_Movement.h>
#include <Game\AIController\AIController.h>
#include <Engine\GUI\Button.h>
#include <Engine\GUI\Text.h>
#include <Engine\GUI\Textbox.h>
#include <Engine\GUI\Tile.h>

PrefabLoader::PrefabLoader(const char * pc_FileName_In, Loader * po_Loader_In, StatsLoader* po_StatsLoader_In)
{
	po_SceneLoader = po_Loader_In;
	po_StatsLoader = po_StatsLoader_In;

	tinyxml2::XMLDocument object_File;
	object_File.LoadFile(pc_FileName_In);
	tinyxml2::XMLElement* body = object_File.FirstChildElement("objects");

	for (tinyxml2::XMLElement* it = body->FirstChildElement("new_Object3D"); it != nullptr; it = it->NextSiblingElement("new_Object3D"))
	{
		std::cout << "Adding new object to the scene..." << "\n";

		//Extract data
		std::vector<std::string> vs_Children;
		std::string s_ObjectName = it->Attribute("name");
		std::string s_Container = it->Attribute("container");

		if (s_Container == "Yes") // This object has children
		{
			//Add variables		
			std::string s_Components = it->Attribute("component");
			glm::vec3 v3_Origin = to3DVector(it->Attribute("origin"));
			glm::vec3 v3_Position = to3DVector(it->Attribute("position"));
			glm::quat quat_Orientation = toQuat(it->Attribute("orientation"));
			glm::vec3 v3_Scale = to3DVector(it->Attribute("scale"));
			std::string s_Children = it->Attribute("children");
			std::string s_Tag = it->Attribute("tag");
			std::string s_StatsName = it->Attribute("stats");

			//Typical process of adding new 3D object
			auto object = new GameObject_3D();
			mipo_Prefabs.insert(std::pair<std::string, Game_Object*>(s_ObjectName, object));
			object->set_Name(s_ObjectName);
			object->set_Prefab(s_ObjectName);
			object->add_Component("Transform_3D", new Transform_3D());

			if (s_StatsName != "" && po_StatsLoader != nullptr)
			{
				object->add_Component("Respond_Movement", new Respond_Movement());
				object->add_Component("Character", new Character(po_StatsLoader->get_Stat(s_StatsName)));
				object->add_Component("RigidBody", new RigidBody(po_StatsLoader->get_Stat(s_StatsName)));
			}

			object->set_RenderStatus(false);
			object->set_Position(v3_Position);
			object->set_Origin(v3_Origin);
			object->set_Rotation(quat_Orientation);
			object->set_Scale(v3_Scale);
			object->set_Tag(s_Tag);
			
			if (s_Children != "") // Make sure that children are really there
			{
				//Add child to the object
				add_Children(vs_Children, s_Children);
				for (auto it = vs_Children.begin(); it != vs_Children.end(); ++it) object->add_ChildName(*it);
			}
		}
		else // It doesn't
		{
			//Add variables
			std::string s_Components = it->Attribute("component");
			std::string i_MeshID = it->Attribute("mesh_ID");
			std::string i_DiffuseID = it->Attribute("diffuse_ID");
			std::string i_SpecularID = it->Attribute("specular_ID");
			glm::vec2 v2_Tiling = to2DVector(it->Attribute("texture_Tiling"));
			std::string s_Tag = it->Attribute("tag");
			float f_Shiny = std::strtof(it->Attribute("shininess"), nullptr);
			glm::vec3 origin = to3DVector(it->Attribute("origin"));
			glm::vec3 position = to3DVector(it->Attribute("position"));
			glm::quat quat_Orientation = toQuat(it->Attribute("orientation"));
			glm::vec3 scale = to3DVector(it->Attribute("scale"));
			std::string s_StatsName = it->Attribute("stats");

			//Typical process of adding new 3D object
			auto object = new GameObject_3D();
			mipo_Prefabs.insert(std::pair<std::string, Game_Object*>(s_ObjectName, object));
			object->set_Name(s_ObjectName);
			object->set_Prefab(s_ObjectName);
			object->add_Component("Mesh_3D", po_Loader_In->get_Mesh(i_MeshID));
			object->add_Component("Transform_3D", new Transform_3D());
			object->add_Component("RenderComp_3D", new RenderComp_3D());
			if (po_StatsLoader_In != nullptr && s_Components != "") add_Components(object, s_Components, po_StatsLoader->get_Stat(s_StatsName));
			object->set_Position(position);
			object->set_Origin(origin);
			object->set_Rotation(quat_Orientation);
			object->set_Scale(scale);
			object->add_Texture("Diffuse_Map", po_Loader_In->get_Texture(i_DiffuseID));
			object->add_Texture("Specular_Map", po_Loader_In->get_Texture(i_SpecularID));
			object->set_Tiles(v2_Tiling);
			object->set_Shininess(f_Shiny);
			object->set_Tag(s_Tag);
		}
	}
	for (tinyxml2::XMLElement* it = body->FirstChildElement("new_ObjectParticle"); it != nullptr; it = it->NextSiblingElement("new_ObjectParticle"))
	{
		std::cout << "Adding new particles to the scene..." << "\n";
		std::string s_ObjectName = it->Attribute("name");
		//Add variables
		std::string s_Components = it->Attribute("component");
		std::string i_MeshID = it->Attribute("mesh_ID");
		std::string i_DiffuseID = it->Attribute("diffuse_ID");
		glm::vec2 v2_Tiling = to2DVector(it->Attribute("texture_Tiling"));
		std::string s_Tag = it->Attribute("tag");
		glm::vec3 origin = to3DVector(it->Attribute("origin"));
		glm::vec3 position = to3DVector(it->Attribute("position"));
		glm::quat quat_Orientation = toQuat(it->Attribute("orientation"));
		glm::vec3 scale = to3DVector(it->Attribute("scale"));

		//Typical process of adding new 3D object
		mipo_Prefabs.insert(std::pair<std::string, Game_Object*>(s_ObjectName, new GameObject_Instanced()));
		auto object = static_cast<GameObject_Instanced*>(mipo_Prefabs.find(s_ObjectName)->second);
		object->set_Name(s_ObjectName);
		object->set_Prefab(s_ObjectName);
		object->add_Component("Mesh_Instanced", po_Loader_In->get_Mesh(i_MeshID));
		object->add_Component("Transform_Instanced", new Transform_Instanced());
		object->add_Component("RenderComp_Instanced", new RenderComp_Instanced());

		object->set_Position(position);
		object->set_Origin(origin);
		object->set_Rotation(quat_Orientation);
		object->set_Scale(scale);
		object->add_Texture("Diffuse_Map", po_Loader_In->get_Texture(i_DiffuseID));
		object->set_Tiles(v2_Tiling);

		object->set_Tag(s_Tag);


	}

	for (tinyxml2::XMLElement* it = body->FirstChildElement("new_Object2D"); it != nullptr; it = it->NextSiblingElement("new_Object2D"))
	{
		std::cout << "Adding new GUI element to the scene..." << "\n";

		//Extract data
		std::string s_GUIName = it->Attribute("name");
		std::string s_GUIType = it->Attribute("type");
		std::string s_FontID = it->Attribute("font_ID");
		std::string s_MeshID = it->Attribute("mesh_ID");
		std::string s_TextureID = it->Attribute("texture_ID");

		if (s_GUIType == "Button") mipo_Prefabs.insert(std::make_pair(s_GUIName, new Button(0, "", po_Loader_In->get_Font(s_FontID), po_Loader_In->get_Mesh(s_MeshID), po_Loader_In->get_Texture(s_TextureID), glm::vec2(0.f), glm::vec2(0.f), 0.f, 0.f, false, "")));
		else if (s_GUIType == "Text") mipo_Prefabs.insert(std::make_pair(s_GUIName, new Text(0, "", glm::vec2(0.f), 0.f, po_Loader_In->get_Font(s_FontID), 0.f, false, "")));
		else if (s_GUIType == "Textbox") mipo_Prefabs.insert(std::make_pair(s_GUIName, new Textbox(0, po_Loader_In->get_Font(s_FontID), po_Loader_In->get_Mesh(s_MeshID), po_Loader_In->get_Texture(s_TextureID), glm::vec2(0.f), glm::vec2(0.f), 0.f, 0.f, false, "")));
		else if (s_GUIType == "Tile") mipo_Prefabs.insert(std::make_pair(s_GUIName, new Tile(0, "", po_Loader_In->get_Mesh(s_MeshID), po_Loader_In->get_Texture(s_TextureID), glm::vec2(0.f), glm::vec2(0.f), 0, 0.f)));
	}


	body = object_File.FirstChildElement("lights");

	for (tinyxml2::XMLElement* it = body->FirstChildElement("new_Light"); it != nullptr; it = it->NextSiblingElement("new_Light"))
	{
		std::cout << "Adding lights to the scene..." << "\n";

		//Add variables
		std::string s_Type;
		glm::vec3 v3_Ambient;
		glm::vec3 v3_Diffuse;
		glm::vec3 v3_Specular;
		glm::vec3 v3_Direction;
		glm::vec3 v3_Position;
		float f_lRadius;
		std::string s_Tag;

		s_Type = it->Attribute("type");
		s_Tag = it->Attribute("tag");
		std::string s_Name = it->Attribute("name");

		if (s_Type == "Directional") // Later
		{
			//TO-DO
		}
		else if (s_Type == "Spotlight") // Later
		{
			//TO-DO
		}
		else if (s_Type == "Point_Light")
		{
			//Point light
			std::string s_Name = it->Attribute("name");
			v3_Position = to3DVector(it->Attribute("position"));
			glm::vec3 v3_Position = to3DVector(it->Attribute("position"));
			v3_Ambient = to3DVector(it->Attribute("ambient"));
			v3_Diffuse = to3DVector(it->Attribute("diffuse"));
			v3_Specular = to3DVector(it->Attribute("specular"));
			f_lRadius = std::strtof(it->Attribute("radius"), nullptr);
			mipo_Prefabs.insert(std::pair<std::string, Game_Object*>(s_Name, new Point_Light(v3_Ambient, v3_Diffuse, v3_Specular, f_lRadius)));
			auto point_Light = static_cast<Point_Light*>(mipo_Prefabs.find(s_Name)->second);
			point_Light->set_Name(s_Name);
			point_Light->add_Component("Mesh_3D", po_Loader_In->get_Mesh("7"));
			point_Light->add_Component("Transform_3D", new Transform_3D());
			point_Light->add_Component("RenderComp_3D", new RenderComp_3D());
			point_Light->set_Position(v3_Position);
			point_Light->set_Scale(glm::vec3(0.5f, 0.5f, 0.5f));
			point_Light->set_Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
			point_Light->set_RenderStatus(true);
			point_Light->add_Texture("Diffuse_Map", po_Loader_In->get_Texture("34"));
			point_Light->add_Texture("Specular_Map", po_Loader_In->get_Texture("34"));
			point_Light->set_Tiles(glm::vec2(1.f, 1.f));
			point_Light->set_Shininess(1.f);
			point_Light->set_Tag(s_Tag);
			point_Light->set_Radius(f_lRadius);
			point_Light->set_Prefab(s_Name);
		}
	}
}

glm::vec3 PrefabLoader::to3DVector(const char * pc_Vector3D_In)
{
	std::string s_Result;
	int i_DataCounter = 0;
	int i_Length = std::strlen(pc_Vector3D_In);
	glm::vec3 v3_Vector;

	for (int i = 0; i < i_Length; i++)
	{
		switch (pc_Vector3D_In[i])
		{
		case 32: // Empty space
				 //Ignore
			break;

		case 44: // Comma
			i_DataCounter++;

			switch (i_DataCounter)
			{
			case 1:
				v3_Vector.x = std::strtof(s_Result.c_str(), NULL);
				s_Result.clear();
				break;

			case 2:
				v3_Vector.y = std::strtof(s_Result.c_str(), NULL);
				s_Result.clear();
				break;
			}
			break;
			break;

		case 40: // This bracker "(" 
				 //Ignore
			break;

		case 41: // This bracket ")"
			v3_Vector.z = std::strtof(s_Result.c_str(), NULL);
			break;

			//Process
		default:
			s_Result += pc_Vector3D_In[i];
			break;
		}
	}

	return glm::vec3(v3_Vector.x, v3_Vector.y, v3_Vector.z);
}

void PrefabLoader::add_Children(std::vector<std::string>& vs_Children_In, std::string s_ToProcess_In)
{
	std::string s_Result;
	int i_Length = s_ToProcess_In.length();
	bool b_IgnoreSpaces = false;

	for (int i = 0; i < i_Length; i++)
	{
		switch (s_ToProcess_In[i])
		{
		case 40: // This bracket "("
				 //Ignore
			break;

		case 41: // This bracket ")"
			vs_Children_In.push_back(s_Result);
			s_Result.clear();
			b_IgnoreSpaces = true;
			break;

		case 44:  // Comma
			vs_Children_In.push_back(s_Result);
			s_Result.clear();
			b_IgnoreSpaces = true;
			break;

		case '\n':
			break;

		case '\t':
			break;

		case 32:
			if (!b_IgnoreSpaces) s_Result = s_Result + s_ToProcess_In[i];
			break;

			//Process
		default:
			s_Result = s_Result + s_ToProcess_In[i];
			if (s_Result.length() > 1) b_IgnoreSpaces = false;

			break;
		}
	}
}

glm::vec2 PrefabLoader::to2DVector(const char * pc_Vector2D_In)
{
	std::string s_Result;
	int i_DataCounter = 0;
	int i_Length = std::strlen(pc_Vector2D_In);
	glm::vec2 v2_Vector;

	for (int i = 0; i < i_Length; i++)
	{
		switch (pc_Vector2D_In[i])
		{
		case 32: // Empty space
				 //Ignore
			break;

		case 44: // Comma
			v2_Vector.x = std::strtof(s_Result.c_str(), nullptr);
			s_Result.clear();
			break;

		case 40: // This bracker "(" 
				 //Ignore
			break;

		case 41: // This bracket ")"
			v2_Vector.y = std::strtof(s_Result.c_str(), nullptr);
			break;

			//Process
		default:
			s_Result += pc_Vector2D_In[i];
			break;
		}
	}

	return glm::vec2(v2_Vector.x, v2_Vector.y);
}

glm::quat PrefabLoader::toQuat(const char * pc_Quaternion_In)
{
	std::string s_Result;
	int i_DataCounter = 0;
	int i_Length = std::strlen(pc_Quaternion_In);
	//float f_Angle;
	//glm::vec3 v3_Vector;
	glm::quat temp;
	for (int i = 0; i < i_Length; i++)
	{
		switch (pc_Quaternion_In[i])
		{
		case 32: // Empty space
				 //Ignore
			break;

		case 44: // Comma
			i_DataCounter++;

			switch (i_DataCounter)
			{
			case 1:
				temp.w = std::strtof(s_Result.c_str(), nullptr);
				s_Result.clear();
				break;

			case 2:
				temp.x = std::strtof(s_Result.c_str(), nullptr);
				s_Result.clear();
				break;

			case 3:
				temp.y = std::strtof(s_Result.c_str(), nullptr);
				s_Result.clear();
				break;
			}
			break;

		case 40: // This bracket "("
				 //Ignore
			break;

		case 41: // This bracket ")"
			temp.z = std::strtof(s_Result.c_str(), nullptr);
			break;

			//Process
		default:
			s_Result = s_Result + pc_Quaternion_In[i];
			break;
			break;
		}
	}


	return temp;
}

Game_Object * PrefabLoader::get_Prefab(std::string s_Name_In)
{
	return mipo_Prefabs.find(s_Name_In)->second;
}

std::map<std::string, Game_Object*>& PrefabLoader::get_PrefabMap()
{
	return mipo_Prefabs;
}

void PrefabLoader::clean_Up()
{
	for (const auto& pair : mipo_Prefabs) delete pair.second;
}

void PrefabLoader::identify_Component(GameObject_3D* po_GameObject_In, std::string& s_ToProcess_In, Stats * stat_In)
{
	std::cout << "Component name: " << s_ToProcess_In << "\n";
	if (s_ToProcess_In == "Mesh_3D") std::cout << "Nope" << "\n";
	else if (s_ToProcess_In == "AI_Controller") { po_GameObject_In->add_Component("AI_Controller", new AIController(po_GameObject_In)); }
	else if (s_ToProcess_In == "Respond_Movement") po_GameObject_In->add_Component("Respond_Movement", new Respond_Movement());
	else if (s_ToProcess_In == "BoxCollider_3D") po_GameObject_In->add_Component("BoxCollider_3D", new BoxCollider_3D());
	else if (s_ToProcess_In == "RigidBody")
		if (stat_In != nullptr) po_GameObject_In->add_Component("RigidBody", new RigidBody(stat_In));
		else po_GameObject_In->add_Component("RigidBody", new RigidBody());
	else if (s_ToProcess_In == "Character") po_GameObject_In->add_Component("Character", new Character(stat_In));

	s_ToProcess_In.clear();
}

void PrefabLoader::add_Components(GameObject_3D* po_GameObject_In, std::string s_ToProcess_In, Stats * stats_In)
{
	std::string s_Result;
	int i_Length = s_ToProcess_In.length();

	for (int i = 0; i < i_Length; i++)
	{
		switch (s_ToProcess_In[i])
		{
		case 40: // This bracket "("
				 //Ignore
			break;

		case 41: // This bracket ")"
			identify_Component(po_GameObject_In, s_Result, stats_In);
			break;

		case 44:  // Comma
				  //Find the right component
			identify_Component(po_GameObject_In, s_Result, stats_In);
			break;

		case '\n':
			break;

		case '\t':
			break;

		case 32:
			break;

			//Process
		default:
			s_Result = s_Result + s_ToProcess_In[i];
			break;
		}
	}
}