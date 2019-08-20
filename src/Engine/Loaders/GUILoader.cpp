#include <Engine\Game_Objects\GameObject_2D.h>
#include <Engine\GUI\Button.h>
#include <Engine\GUI\Text.h>
#include <Engine\GUI\Textbox.h>
#include <Engine\GUI\Tile.h>
#include <Engine\Loaders\PrefabLoader.h>
#include <Engine\Loaders\GUILoader.h>
#include <TinyXML2\tinyxml2.h>
#include <iostream>
#include <string>

GUILoader::GUILoader(const char * pc_FileName_In, PrefabLoader * po_PrefLoader_In, std::map<std::string, Game_Object*>& mspo_GameObjects_In, glm::vec2 v2_WindowSize_In)
{
	v2_WindowSize = v2_WindowSize_In;

	tinyxml2::XMLDocument object_File;
	object_File.LoadFile(pc_FileName_In);
	tinyxml2::XMLElement* body = object_File.FirstChildElement("gui");

	for (tinyxml2::XMLElement* it = body->FirstChildElement("new_GUIElement"); it != nullptr; it = it->NextSiblingElement("new_GUIElement"))
	{
		std::cout << "Adding new GUI element to the scene..." << "\n";

		//Extract data
		std::string s_GUIName = it->Attribute("name");
		int i_PickID = std::atoi(it->Attribute("pick_ID"));
		std::string s_ObjectType = it->Attribute("object_Type");
		std::string s_GUIType = it->Attribute("gui_Type");
		std::string s_Prefab = it->Attribute("prefab");
		std::string s_Text = it->Attribute("text");
		std::string s_Render = it->Attribute("render");
		float f_Angle = std::strtof(it->Attribute("angle"), nullptr);
		float f_Layer = std::strtof(it->Attribute("layer"), nullptr);
		glm::vec2 v2_Position = to2DVector(it->Attribute("position"));
		std::string s_WindowPosMode = it->Attribute("window_PosMode");
		glm::vec2 v2_Size = to2DVector(it->Attribute("size"));
		std::string s_WindowSizeMode = it->Attribute("window_SizeMode");

		if (s_GUIType == "Button")
		{
			auto desired_Prefab = static_cast<Button*>(po_PrefLoader_In->get_Prefab(s_Prefab));
			mspo_GameObjects_In.insert(std::make_pair(s_GUIName, new Button(*desired_Prefab)));
			auto found_GUIElement = static_cast<Button*>(mspo_GameObjects_In.find(s_GUIName)->second);
			found_GUIElement->set_ObjectID(i_PickID);
			found_GUIElement->set_ObjectType(s_ObjectType);
			if (s_Render == "true") found_GUIElement->set_RenderStatus(true);
			else found_GUIElement->set_RenderStatus(false);
			found_GUIElement->set_Angle(f_Angle);
			found_GUIElement->set_Layer(f_Layer);
			found_GUIElement->set_Position(apply_Pattern(found_GUIElement, s_WindowPosMode, v2_Position));
			found_GUIElement->set_Size(apply_Pattern(found_GUIElement, s_WindowSizeMode, v2_Size));	
			found_GUIElement->set_Text(s_Text);
		}
		else if (s_GUIType == "Text")
		{
			auto desired_Prefab = static_cast<Text*>(po_PrefLoader_In->get_Prefab(s_Prefab));
			mspo_GameObjects_In.insert(std::make_pair(s_GUIName, new Text(*desired_Prefab)));
			auto found_GUIElement = static_cast<Text*>(mspo_GameObjects_In.find(s_GUIName)->second);
			found_GUIElement->set_ObjectID(i_PickID);
			found_GUIElement->set_ObjectType(s_ObjectType);
			if (s_Render == "true") found_GUIElement->set_RenderStatus(true);
			else found_GUIElement->set_RenderStatus(false);
			found_GUIElement->set_Angle(f_Angle);
			found_GUIElement->set_Layer(f_Layer);
			found_GUIElement->set_Position(apply_Pattern(found_GUIElement, s_WindowPosMode, v2_Position));
			found_GUIElement->set_Size(apply_Pattern(found_GUIElement, s_WindowSizeMode, v2_Size));
			found_GUIElement->set_Text(s_Text);
		}
		else if (s_GUIType == "Textbox")
		{
			auto desired_Prefab = static_cast<Textbox*>(po_PrefLoader_In->get_Prefab(s_Prefab));
			mspo_GameObjects_In.insert(std::make_pair(s_GUIName, new Textbox(*desired_Prefab)));
			auto found_GUIElement = static_cast<Textbox*>(mspo_GameObjects_In.find(s_GUIName)->second);
			found_GUIElement->set_ObjectID(i_PickID);
			found_GUIElement->set_ObjectType(s_ObjectType);
			if (s_Render == "true") found_GUIElement->set_RenderStatus(true);
			else found_GUIElement->set_RenderStatus(false);
			found_GUIElement->set_Angle(f_Angle);
			found_GUIElement->set_Layer(f_Layer);
			found_GUIElement->set_Position(apply_Pattern(found_GUIElement, s_WindowPosMode, v2_Position));
			found_GUIElement->set_Size(apply_Pattern(found_GUIElement, s_WindowSizeMode, v2_Size));
		}
		else if (s_GUIType == "Tile")
		{
			auto desired_Prefab = static_cast<Tile*>(po_PrefLoader_In->get_Prefab(s_Prefab));
			mspo_GameObjects_In.insert(std::make_pair(s_GUIName, new Tile(*desired_Prefab)));
			auto found_GUIElement = static_cast<Tile*>(mspo_GameObjects_In.find(s_GUIName)->second);
			found_GUIElement->set_ObjectID(i_PickID);
			found_GUIElement->set_ObjectType(s_ObjectType);
			if (s_Render == "true") found_GUIElement->set_RenderStatus(true);
			else found_GUIElement->set_RenderStatus(false);
			found_GUIElement->set_Angle(f_Angle);
			found_GUIElement->set_Layer(f_Layer);
			found_GUIElement->set_Position(apply_Pattern(found_GUIElement, s_WindowPosMode, v2_Position));
			found_GUIElement->set_Size(apply_Pattern(found_GUIElement, s_WindowSizeMode, v2_Size));
		}
	}
}

glm::vec2 GUILoader::to2DVector(const char * pc_Vector2D_In)
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

glm::vec2 GUILoader::apply_Pattern(GameObject_2D * p_Object_In, std::string s_WindowMode_In, glm::vec2 v2_Position_In)
{
	if (s_WindowMode_In == "") return v2_Position_In;
	else if (s_WindowMode_In == "UseX_Add")
	{
		glm::vec2 v2_Vector = glm::vec2(v2_Position_In.x + v2_WindowSize.x, v2_Position_In.y);
		return v2_Vector;
	}
	else if (s_WindowMode_In == "UseY_Add")
	{
		glm::vec2 v2_Vector = glm::vec2(v2_Position_In.x, v2_Position_In.y + v2_WindowSize.y);
		return v2_Vector;
	}
	else if (s_WindowMode_In == "UseXY_Add")
	{
		glm::vec2 v2_Vector = glm::vec2(v2_Position_In.x + v2_WindowSize.x, v2_Position_In.y + v2_WindowSize.y);
		return v2_Vector;
	}
	else if (s_WindowMode_In == "UseX_Half")
	{
		glm::vec2 v2_Vector = glm::vec2(v2_Position_In.x + v2_WindowSize.x / 2.f, v2_Position_In.y);
		return v2_Vector;
	}
	else if (s_WindowMode_In == "UseY_Half")
	{
		glm::vec2 v2_Vector = glm::vec2(v2_Position_In.x, v2_Position_In.y + v2_WindowSize.y / 2.f);
		return v2_Vector;
	}
	else if (s_WindowMode_In == "UseXY_Half")
	{
		glm::vec2 v2_Vector = glm::vec2(v2_Position_In.x + v2_WindowSize.x / 2.f, v2_Position_In.y + v2_WindowSize.y / 2.f);
		return v2_Vector;
	}
	
}
