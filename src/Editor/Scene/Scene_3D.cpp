#include <Editor\Scene\Scene_3D.h>
#include <GLFW\glfw3.h>
#include <Engine/Mesh/Mesh_3D.h>
#include <Engine\Game_Objects\GameObject_3D.h>
#include <Engine\Loaders\SceneLoader.h>
#include <Engine/Component/RenderComp_3D.h>
#include <Engine\Component\Transform_3D.h>
#include <Engine\Lighting\Light.h>
#include <iostream>
#include <Editor\Objects\Arrow_3D.h>
#include <Engine\Game_Objects\Camera_2D.h>
#include <Engine\Lighting\Point_Light.h>
#include <Engine\GUI\Button.h>
#include <Engine\GUI\Textbox.h>
#include <Engine\GUI\Text.h>
#include <Engine\GUI\Tile.h>
#include <algorithm>

void Scene_3D::add_Arrows()
{
	glm::quat quat_RedArrow = glm::angleAxis(glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));
	glm::quat quat_GreenArrow = glm::angleAxis(glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
	glm::quat quat_BlueArrow = glm::angleAxis(glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));

	mspo_Objects.insert(std::pair<std::string, Game_Object*>("Red Arrow", new Arrow_3D(1000000, "Red Arrow", static_cast<Mesh_3D*>(po_Loader->get_Mesh("8")), quat_RedArrow, po_Loader->get_Texture("27"), po_Loader->get_Texture("7"))));
	mspo_Objects.insert(std::pair<std::string, Game_Object*>("Green Arrow", new Arrow_3D(1000001, "Green Arrow", static_cast<Mesh_3D*>(po_Loader->get_Mesh("8")), quat_GreenArrow, po_Loader->get_Texture("28"), po_Loader->get_Texture("7"))));
	mspo_Objects.insert(std::pair<std::string, Game_Object*>("Blue Arrow", new Arrow_3D(1000002, "Blue Arrow", static_cast<Mesh_3D*>(po_Loader->get_Mesh("8")), quat_BlueArrow, po_Loader->get_Texture("29"), po_Loader->get_Texture("7"))));
}

void Scene_3D::render_ColourScene()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(po_Loader->get_Shader("3")->get_Program());
	camera_3D->update_Shader(po_Loader->get_Shader("3"));

	for (auto const& pair : mspo_Objects)
	{
		if (pair.second->get_Tag() != "GUI")
		{

			int r = (pair.second->get_ObjectID() & 0x000000FF) >> 0;
			int g = (pair.second->get_ObjectID() & 0x0000FF00) >> 8;
			int b = (pair.second->get_ObjectID() & 0x00FF0000) >> 16;

			GLuint pickingColourID = glGetUniformLocation(po_Loader->get_Shader("3")->get_Program(), "pickColour");
			glUniform4f(pickingColourID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

			pair.second->render(po_Loader->get_Shader("3"));
		}
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	for (auto const& pair : mspo_Objects)
	{
		if (pair.second->get_Tag() == "Arrow")
		{
			int r = (pair.second->get_ObjectID() & 0x000000FF) >> 0;
			int g = (pair.second->get_ObjectID() & 0x0000FF00) >> 8;
			int b = (pair.second->get_ObjectID() & 0x00FF0000) >> 16;

			GLuint pickingColourID = glGetUniformLocation(po_Loader->get_Shader("3")->get_Program(), "pickColour");
			glUniform4f(pickingColourID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

			pair.second->render(po_Loader->get_Shader("3"));
		}
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(po_Loader->get_Shader("4")->get_Program());
	camera_2D->update_Shader(po_Loader->get_Shader("4"));

	for (auto const& pair : mspo_Objects)
	{
		if (pair.second->get_Tag() == "GUI")
		{
			int r = (pair.second->get_ObjectID() & 0x000000FF) >> 0;
			int g = (pair.second->get_ObjectID() & 0x0000FF00) >> 8;
			int b = (pair.second->get_ObjectID() & 0x00FF0000) >> 16;

			GLuint pickingColourID = glGetUniformLocation(po_Loader->get_Shader("4")->get_Program(), "pickColour");
			glUniform4f(pickingColourID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

			pair.second->render(po_Loader->get_Shader("4"));
		}
	}

	glFlush();
	glFinish();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	glReadPixels((v2_WindowSize.x / 2.f) + (v2_MousePos.x - (v2_WindowSize.x / 2.f)), (v2_WindowSize.y / 2.f) - (v2_MousePos.y - (v2_WindowSize.y / 2.f)), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// Convert the color back to an integer ID
	pickedID =
		data[0] +
		data[1] * 256 +
		data[2] * 256 * 256;

	if (pickedID == 0x00ffffff) { // Full white, must be the background !
		std::cout << "background" << "\n";
	}
	else {
		std::cout << "Object with ID:" << pickedID << "\n";
	}
}

std::string Scene_3D::to3DVectorString(glm::vec3 pc_Vector3D_In)
{
	std::string s_Vector3D = "(" + std::to_string(pc_Vector3D_In.x) + ", " + std::to_string(pc_Vector3D_In.y) + ", " + std::to_string(pc_Vector3D_In.z) + ")";

	return s_Vector3D;
}

void Scene_3D::process_3DClick(float f_Delta_In, glm::vec2 v2_MousePos_In)
{
	if (b_Conditions[Conditions::Arrow_Moving] && !b_Conditions[Conditions::Stop_Arrow])
	{
		switch (pickedID)
		{
			//Red Arrow moving
		case 1000000:
			if (picked_Object->get_Parent() != nullptr)
			{
				auto parent = static_cast<GameObject_3D*>(picked_Object->get_Parent());
				parent->set_Position(glm::vec3(parent->get_Position().x + camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x * camera_3D->get_CameraSide(), parent->get_Position().y, parent->get_Position().z));
				static_cast<GameObject_3D*>(mspo_Objects.find("Red Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Green Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Blue Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
			}
			else if (picked_Object->get_Children().empty())
			{
				picked_Object->set_Position(glm::vec3(picked_Object->get_Position().x + camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x * camera_3D->get_CameraSide(), picked_Object->get_Position().y, picked_Object->get_Position().z));
				static_cast<GameObject_3D*>(mspo_Objects.find("Red Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Green Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Blue Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
			}
			break;

			//Green arrow moving
		case 1000001:
			if (picked_Object->get_Parent() != nullptr)
			{
				auto parent = static_cast<GameObject_3D*>(picked_Object->get_Parent());
				parent->set_Position(glm::vec3(parent->get_Position().x, parent->get_Position().y - camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).y, parent->get_Position().z));
				static_cast<GameObject_3D*>(mspo_Objects.find("Red Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Green Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Blue Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
			}
			else if (picked_Object->get_Children().empty())
			{
				picked_Object->set_Position(glm::vec3(picked_Object->get_Position().x, picked_Object->get_Position().y - camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).y, picked_Object->get_Position().z));
				static_cast<GameObject_3D*>(mspo_Objects.find("Red Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Green Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Blue Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
			}
			break;

			//Blue arrow moving
		case 1000002:
			if (picked_Object->get_Parent() != nullptr)
			{
				auto parent = static_cast<GameObject_3D*>(picked_Object->get_Parent());
				parent->set_Position(glm::vec3(parent->get_Position().x, parent->get_Position().y, parent->get_Position().z - camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x));
				static_cast<GameObject_3D*>(mspo_Objects.find("Red Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Green Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Blue Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
			}
			else if (picked_Object->get_Children().empty())
			{
				picked_Object->set_Position(glm::vec3(picked_Object->get_Position().x, picked_Object->get_Position().y, picked_Object->get_Position().z - camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x));
				static_cast<GameObject_3D*>(mspo_Objects.find("Red Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Green Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
				static_cast<GameObject_3D*>(mspo_Objects.find("Blue Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
			}
			break;
		}
	}
	else if (b_Conditions[Conditions::Arrow_Scaling] && !b_Conditions[Conditions::Stop_Arrow])
	{
		switch (pickedID)
		{
			//Red Arrow moving
		case 1000000:
			if (picked_Object->get_Parent() != nullptr)
			{
				auto parent = static_cast<GameObject_3D*>(picked_Object->get_Parent());
				parent->set_Scale(glm::vec3(parent->get_Scale().x + camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x, parent->get_Scale().y, parent->get_Scale().z));
			}
			else if (picked_Object->get_Children().empty())
			{
				picked_Object->set_Scale(glm::vec3(picked_Object->get_Scale().x + camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x, picked_Object->get_Scale().y, picked_Object->get_Scale().z));
			}
			break;

			//Green arrow moving
		case 1000001:
			if (picked_Object->get_Parent() != nullptr)
			{
				auto parent = static_cast<GameObject_3D*>(picked_Object->get_Parent());
				parent->set_Scale(glm::vec3(parent->get_Scale().x, parent->get_Scale().y - camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).y, parent->get_Scale().z));
			}
			else if (picked_Object->get_Children().empty())
			{
				picked_Object->set_Scale(glm::vec3(picked_Object->get_Scale().x, picked_Object->get_Scale().y - camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).y, picked_Object->get_Scale().z));
			}
			break;

			//Blue arrow moving
		case 1000002:
			if (picked_Object->get_Parent() != nullptr)
			{
				auto parent = static_cast<GameObject_3D*>(picked_Object->get_Parent());
				parent->set_Scale(glm::vec3(parent->get_Scale().x, parent->get_Scale().y, parent->get_Scale().z + camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x));
			}
			else if (picked_Object->get_Children().empty())
			{
				picked_Object->set_Scale(glm::vec3(picked_Object->get_Scale().x, picked_Object->get_Scale().y, picked_Object->get_Scale().z + camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x));
			}
			break;
		}
	}
	else if (b_Conditions[Conditions::Arrow_Rotation] && !b_Conditions[Conditions::Stop_Arrow])
	{
		switch (pickedID)
		{
			//Red Arrow moving
		case 1000000:

			if (picked_Object->get_Parent() != nullptr)
			{
				auto parent = static_cast<GameObject_3D*>(picked_Object->get_Parent());

				glm::quat other_Quat = glm::normalize(glm::angleAxis(glm::radians(camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x) * camera_3D->get_CameraSide(), glm::vec3(0.f, 0.f, -1.f)));
				parent->set_Rotation(other_Quat);
			}
			else if (picked_Object->get_Children().empty())
			{
				glm::quat other_Quat = glm::normalize(glm::angleAxis(glm::radians(camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x) * camera_3D->get_CameraSide(), glm::vec3(0.f, 0.f, -1.f)));
				picked_Object->set_Rotation(other_Quat);
			}
			break;

			//Green arrow moving
		case 1000001:
			if (picked_Object->get_Parent() != nullptr)
			{
				auto parent = static_cast<GameObject_3D*>(picked_Object->get_Parent());
				glm::quat other_Quat = glm::normalize(glm::angleAxis(glm::radians(camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x) * camera_3D->get_CameraSide(), glm::vec3(0.f, -1.f, 0.f)));
				parent->set_Rotation(other_Quat);
			}
			else if (picked_Object->get_Children().empty())
			{
				glm::quat other_Quat = glm::normalize(glm::angleAxis(glm::radians(camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x) * camera_3D->get_CameraSide(), glm::vec3(0.f, -1.f, 0.f)));
				picked_Object->set_Rotation(other_Quat);
			}
			break;

			//Blue arrow moving
		case 1000002:
			if (picked_Object->get_Parent() != nullptr)
			{
				auto parent = static_cast<GameObject_3D*>(picked_Object->get_Parent());
				glm::quat other_Quat = glm::normalize(glm::angleAxis(glm::radians(camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x), glm::vec3(-1.f, 0.f, 0.f)));
				parent->set_Rotation(other_Quat);

			}
			else if (picked_Object->get_Children().empty())
			{
				glm::quat other_Quat = glm::normalize(glm::angleAxis(glm::radians(camera_3D->get_PitchYaw(f_Delta_In, v2_MousePos_In, v2_WindowSize).x), glm::vec3(-1.f, 0.f, 0.f)));
				picked_Object->set_Rotation(other_Quat);
			}
			break;
		}
	}
	else
	{
		pickedID = -1;
	}
}

glm::vec3 Scene_3D::to3DVector(const char * pc_Vector3D_In)
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

void Scene_3D::process_2DClick(int i_ID_In)
{
	switch (i_ID_In)
	{
	case 1000: // Add object button
		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Add") pair.second->set_RenderStatus(true);
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Show") pair.second->set_RenderStatus(false);
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Set") pair.second->set_RenderStatus(false);
		}

		break;
	case 1001: // Move button
		b_Conditions[Conditions::Arrow_Moving] = true;
		b_Conditions[Conditions::Arrow_Scaling] = false;
		b_Conditions[Conditions::Arrow_Rotation] = false;
		b_Conditions[Conditions::Selection] = false;
		b_Conditions[Conditions::ListenToKeyboard] = false;

		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Add") pair.second->set_RenderStatus(false);
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Show") pair.second->set_RenderStatus(false);
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Set") pair.second->set_RenderStatus(false);
		}

		break;

	case 1002: //Rotate button
		b_Conditions[Conditions::Arrow_Moving] = false;
		b_Conditions[Conditions::Arrow_Scaling] = false;
		b_Conditions[Conditions::Arrow_Rotation] = true;
		b_Conditions[Conditions::Selection] = false;
		b_Conditions[Conditions::ListenToKeyboard] = false;

		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Add") pair.second->set_RenderStatus(false);
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Show") pair.second->set_RenderStatus(false);
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Set") pair.second->set_RenderStatus(false);
		}

		break;

	case 1003: // Scale button
		b_Conditions[Conditions::Arrow_Moving] = false;
		b_Conditions[Conditions::Arrow_Scaling] = true;
		b_Conditions[Conditions::Arrow_Rotation] = false;
		b_Conditions[Conditions::Selection] = false;
		b_Conditions[Conditions::ListenToKeyboard] = false;

		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Add") pair.second->set_RenderStatus(false);
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Show") pair.second->set_RenderStatus(false);
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Set") pair.second->set_RenderStatus(false);
		}
		break;

	case 1004: // Selection button
		b_Conditions[Conditions::Arrow_Moving] = false;
		b_Conditions[Conditions::Arrow_Scaling] = false;
		b_Conditions[Conditions::Arrow_Rotation] = false;
		b_Conditions[Conditions::Selection] = true;
		b_Conditions[Conditions::ListenToKeyboard] = false;

		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Add") pair.second->set_RenderStatus(false);
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Show") pair.second->set_RenderStatus(true);
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Set") pair.second->set_RenderStatus(false);
		}
		break;


	case 1028: // Set button
		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Add") pair.second->set_RenderStatus(false);
			else if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Show") pair.second->set_RenderStatus(false);
			else if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Set") pair.second->set_RenderStatus(true);
		}
		break;

	case 1006: // Textbox add name!
		b_Conditions[Conditions::ListenToKeyboard] = true;
		break;
	case 1008: // Textbox add name!
		b_Conditions[Conditions::ListenToKeyboard] = true;
		break;
	case 1010: // Textbox add name!
		b_Conditions[Conditions::ListenToKeyboard] = true;
		break;
	case 1012: // Textbox add name!
		b_Conditions[Conditions::ListenToKeyboard] = true;
		break;
	case 1014: // Textbox add name!
		b_Conditions[Conditions::ListenToKeyboard] = true;
		break;

	case 1029: // Textbox add name!
		b_Conditions[Conditions::ListenToKeyboard] = true;
		break;
	case 1031: // Textbox add name!
		b_Conditions[Conditions::ListenToKeyboard] = true;
		break;
	case 1033: // Textbox add name!
		b_Conditions[Conditions::ListenToKeyboard] = true;
		break;
	case 1038: // Textbox add name!
		b_Conditions[Conditions::ListenToKeyboard] = true;
		break;
	}

	if (i_ID_In == 1025)
	{
		bool b_IsEmpty[5];
		glm::vec3 v3_Origin;
		glm::vec3 v3_Position;
		glm::vec3 v3_Scale;
		std::string s_Name;
		std::string s_PrefabName;
		b_IsEmpty[0] = true;
		for (auto const& pair : mspo_Objects) if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Add")
			{
				if (pair.first == "Add_ObjectTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					s_Name = textBox->get_Text();
					b_IsEmpty[0] = false;
				}
				else if (pair.first == "Add_PrefabTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					s_PrefabName = textBox->get_Text();
					b_IsEmpty[1] = false;
				}
				else if (pair.first == "Add_OriginTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					v3_Origin = to3DVector(textBox->get_Text().c_str());
					b_IsEmpty[2] = false;
				}
				else if (pair.first == "Add_PositionTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					v3_Position = to3DVector(textBox->get_Text().c_str());
					b_IsEmpty[3] = false;
				}
				else if (pair.first == "Add_ScaleTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					v3_Scale = to3DVector(textBox->get_Text().c_str());
					b_IsEmpty[4] = false;
				}
			}
		
		auto does_ObjectExist = mspo_Objects.find(s_Name);
		auto does_PrefabExist = po_PrefabLoader->get_PrefabMap().find(s_PrefabName);

		if (does_ObjectExist == mspo_Objects.end() && does_PrefabExist != po_PrefabLoader->get_PrefabMap().end())
		{
			if (s_PrefabName != "Main Light" && s_PrefabName != "Lamp Light")
			{
				if (!b_IsEmpty[0] && !b_IsEmpty[1] && !b_IsEmpty[2] && !b_IsEmpty[3] && !b_IsEmpty[4])
				{
					auto desired_Prefab = static_cast<GameObject_3D*>(po_PrefabLoader->get_Prefab(s_PrefabName));
					mspo_Objects.insert(std::pair<std::string, Game_Object*>(s_Name, new GameObject_3D(*desired_Prefab)));
					auto desired_Object = static_cast<GameObject_3D*>(mspo_Objects.find(s_Name)->second);

					for (int i = 0; i < desired_Prefab->get_ChildrenNames().size(); i++)
					{
						std::string s_ChildName = s_Name + " Child " + std::to_string(i_Counter + i);

						auto prefab_Child = static_cast<GameObject_3D*>(po_PrefabLoader->get_Prefab(desired_Prefab->get_ChildrenNames()[i]));
						mspo_Objects.insert(std::pair<std::string, GameObject_3D*>(s_ChildName, new GameObject_3D(*prefab_Child)));
						auto found_Child = mspo_Objects.find(s_ChildName)->second;
						found_Child->set_Name(s_ChildName);
						found_Child->set_ObjectID(i_Counter + i);
						found_Child->set_Tag("Object_NonSavable");
						desired_Object->add_Child(found_Child);
						desired_Object->add_ChildName(desired_Prefab->get_ChildrenNames()[i]);
					}

					desired_Object->set_Name(s_Name);
					desired_Object->set_Position(v3_Position);
					desired_Object->set_Scale(v3_Scale);
					desired_Object->set_Origin(v3_Origin);
					desired_Object->set_ObjectID(i_Counter);
					//if (s_PrefabName == "Main Light" || s_PrefabName == "Lamp Light")
					//{
					//	light_ID++;
					//	static_cast<Point_Light*>(desired_Object)->set_ID(light_ID);
					//}
					b_Conditions[Conditions::Selection] = true;
					pickedID = desired_Object->get_ObjectID();
					i_Counter++;
				}
			}
			else
			{
				auto desired_Prefab = static_cast<Point_Light*>(po_PrefabLoader->get_Prefab(s_PrefabName));
				mspo_Objects.insert(std::pair<std::string, Game_Object*>(s_Name, new Point_Light(*desired_Prefab)));
				auto desired_Object = static_cast<Point_Light*>(mspo_Objects.find(s_Name)->second);

				desired_Object->set_Name(s_Name);
				desired_Object->set_Position(v3_Position);
				desired_Object->set_ObjectID(i_Counter);

				light_ID++;
				desired_Object->set_ID(light_ID);

				b_Conditions[Conditions::Selection] = true;
				pickedID = desired_Object->get_ObjectID();
				i_Counter++;


				glUseProgram(po_Loader->get_Shader("0")->get_Program());
				o_SceneLoader->set_LightAmount(po_Loader->get_Shader("0"), true);
	
			}
		}
		else pickedID = -1;

	}
	if (i_ID_In == 1026)
	{
		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Add")
			{
				if (pair.first == "Add_ObjectTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					textBox->set_Text("");
				}
				else if (pair.first == "Add_PrefabTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					textBox->set_Text("");
				}
				else if (pair.first == "Add_OriginTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					textBox->set_Text("");
				}
				else if (pair.first == "Add_PositionTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					textBox->set_Text("");
				}
				else if (pair.first == "Add_ScaleTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					textBox->set_Text("");
				}

				pair.second->set_RenderStatus(false);
			}
		}
	}
	if (i_ID_In == 1034)
	{
		bool b_IsEmpty[3];
		glm::vec3 v3_Origin;
		glm::vec3 v3_Position;
		glm::vec3 v3_Scale;
		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Set")
			{
				if (pair.first == "Set_OriginTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					v3_Origin = to3DVector(textBox->get_Text().c_str());
					if (textBox->get_Text() != "") picked_Object->set_Origin(v3_Origin);
				}
				else if (pair.first == "Set_PositionTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					v3_Position = to3DVector(textBox->get_Text().c_str());
					if (textBox->get_Text() != "") picked_Object->set_Position(v3_Position);
				}
				else if (pair.first == "Set_ScaleTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					v3_Scale = to3DVector(textBox->get_Text().c_str());
					if (textBox->get_Text() != "") picked_Object->set_Scale(v3_Scale);
				}
			}
		}

		pickedID = -1;
	}
	if (i_ID_In == 1035)
	{
		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Hidden_Set")
			{
				if (pair.first == "Set_OriginTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					textBox->set_Text("");
				}
				else if (pair.first == "Set_PositionTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					textBox->set_Text("");
				}
				else if (pair.first == "Set_ScaleTextbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					textBox->set_Text("");
				}

				pair.second->set_RenderStatus(false);
			}
		}
	}
	if (i_ID_In == 1036)
	{
		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Universal")
			{
				if (pair.first == "SaveLoad_Textbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					if (textBox->get_Text() != "")
					{
						b_Conditions[Conditions::ReloadScene] = true;
						s_Directory = "assets/scenes/" + textBox->get_Text();
						pickedID = -1;
						break;
					}
				}
			}
		}
	}

	if (i_ID_In == 1037)
	{
		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "GUI" && pair.second->get_ObjectType() == "Universal")
			{
				if (pair.first == "SaveLoad_Textbox")
				{
					auto textBox = static_cast<Textbox*>(pair.second);
					std::string s_Directory = "assets/scenes/" + textBox->get_Text();
					if (textBox->get_Text() != "") save->save(mspo_Objects, s_Directory);
					pickedID = -1;
				}
			}
		}
	}

}

//Initialize everything once
void Scene_3D::init()
{
	//Initialize
	light_ID = 2;
	lock_mouse(true);
	b_Conditions[0] = false;
	b_Conditions[Conditions::Selection] = true;
	b_Conditions[Conditions::ListenToKeyboard] = false;

	camera_3D = new Camera_3D(45.f, v2_WindowSize.x / v2_WindowSize.y, 0.1f, 1000.f);
	camera_3D->set_CameraPos(glm::vec3(0.f, -20.f, 0.f));
	camera_2D = new Camera_2D(0, v2_WindowSize.x, v2_WindowSize.y, 0);
	if (s_Directory == "") s_Directory = "assets/scenes/Robot_Scene.xml";
	//Load the scene
	po_StatsLoader = nullptr;
	po_PrefabLoader = new PrefabLoader("assets/Prefabs.xml", po_Loader, po_StatsLoader);
	po_GUILoader = new GUILoader("assets/gui/GUI_Editor.xml", po_PrefabLoader, mspo_Objects, v2_WindowSize);
	o_SceneLoader = new SceneLoader(s_Directory.c_str(), po_Loader, po_PrefabLoader, mspo_Objects);

	save = new SceneSaver();
	f_Speed = 0.f;
	i_Counter = o_SceneLoader->get_Count();
	add_Arrows();

	//add lighting
	glUseProgram(po_Loader->get_Shader("6")->get_Program());
	int num = 0;
	int posNum = 0;
	ui_light_Amount = 0;
	for (auto const& pair : mspo_Objects)
	{
		if (pair.second->get_Tag() != "GUI" && pair.second->get_Tag() != "Arrow")
		{
			pos[posNum] = static_cast<GameObject_3D*>(pair.second)->get_Position();

			if (pair.second->get_Tag() == "Light")
			{
				ui_light_Amount++;
				num++;
			}
			posNum++;
		}
	}

	for (unsigned int i = 0; i < ui_light_Amount; i++)
	{
		light[i] = o_SceneLoader->get_LightPosition(i);
		radius[i] = o_SceneLoader->get_LightRadius(1);
	}
	for (unsigned int i = 0; i < 3; i++)
	{
		depth[i] = o_SceneLoader->setup_FBO();
	}

	glUseProgram(po_Loader->get_Shader("0")->get_Program());
	o_SceneLoader->set_LightAmount(po_Loader->get_Shader("0"), false);

	GLint depth_Cube_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "depthMap[0]");
	glUniform1i(depth_Cube_Loc, 2);

	depth_Cube_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "depthMap[1]");
	glUniform1i(depth_Cube_Loc, 3);

	depth_Cube_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "depthMap[2]");
	glUniform1i(depth_Cube_Loc, 4);

	GLint diff_Tex_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "diffuse");
	glUniform1i(diff_Tex_Loc, 0);
	GLint spec_Tex_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "specular");
	glUniform1i(spec_Tex_Loc, 1);

	GLint far_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), "farPlane");
	glUniform1f(far_Loc, 1000.0f);



	b_Init = true;
}

//Do something with keyboard input
void Scene_3D::keyboard_Input(GLfloat f_Delta_In, GLboolean* pab_KeyArray_In, GLboolean* pab_LockedKeys_In, int i_KeyPress_In)
{
	if (b_Conditions[Conditions::ListenToKeyboard])
	{
		if (pab_KeyArray_In[GLFW_KEY_LEFT_SHIFT] && !pab_LockedKeys_In[GLFW_KEY_LEFT_SHIFT]) b_Conditions[Conditions::ShiftDetected] = true;

		if (pab_KeyArray_In[i_KeyPress_In] && !pab_LockedKeys_In[i_KeyPress_In] && i_KeyPress_In != GLFW_KEY_LEFT_SHIFT)
		{
			pab_LockedKeys_In[i_KeyPress_In] = true;
			//std::cout << i_KeyPress_In << "\n";

			for (auto const& pair : mspo_Objects)
			{
				if (pair.second->get_ObjectID() == pickedID && pair.second->get_Tag() == "GUI") // We got it
				{
					auto text_Box = static_cast<Textbox*>(pair.second);

					if (i_KeyPress_In == GLFW_KEY_BACKSPACE && !text_Box->is_LineEmpty())
					{
						text_Box->erase_Letter();
					}
					else if (i_KeyPress_In == GLFW_KEY_BACKSPACE && text_Box->is_LineEmpty())
					{
						text_Box->delete_Line();
					}
					else if (i_KeyPress_In == GLFW_KEY_ENTER)
					{
						b_Conditions[Conditions::ListenToKeyboard] = false;
						pickedID = -1; // Stop pointing at this textbox
						break;
					}
					else if (b_Conditions[Conditions::ShiftDetected] && i_KeyPress_In == GLFW_KEY_9)
					{
						text_Box->add_Letter("(");
					}
					else if (b_Conditions[Conditions::ShiftDetected] && i_KeyPress_In == GLFW_KEY_0)
					{
						text_Box->add_Letter(")");
					}
					else if (b_Conditions[Conditions::ShiftDetected] && i_KeyPress_In == 45)
					{
						text_Box->add_Letter("_");
					}
					else if (i_KeyPress_In == GLFW_KEY_TAB || i_KeyPress_In == GLFW_KEY_CAPS_LOCK || i_KeyPress_In == 348 ||
						i_KeyPress_In == GLFW_KEY_UP || i_KeyPress_In == GLFW_KEY_DOWN || i_KeyPress_In == GLFW_KEY_LEFT ||
						i_KeyPress_In == GLFW_KEY_RIGHT || i_KeyPress_In == 92 || i_KeyPress_In == 96 || i_KeyPress_In == 267)
					{
						//Ignore
					}
					else
					{
						std::string s_Letter;

						s_Letter.push_back(i_KeyPress_In);
						if (!b_Conditions[Conditions::ShiftDetected]) std::transform(s_Letter.begin(), s_Letter.end(), s_Letter.begin(), ::tolower);
						text_Box->add_Letter(s_Letter);
					}
				}
			}

		}
		if (!pab_KeyArray_In[i_KeyPress_In]) pab_LockedKeys_In[i_KeyPress_In] = false;
	}
	else
	{
		float f_MagicNumber = 0.7071f;

		if (pab_KeyArray_In[GLFW_KEY_R] && !pab_LockedKeys_In[GLFW_KEY_R])
		{
			reload_Scene();
			pab_LockedKeys_In[GLFW_KEY_R] = true;
		}
		if (!pab_KeyArray_In[GLFW_KEY_R]) pab_LockedKeys_In[GLFW_KEY_R] = false;

		if (pab_KeyArray_In[GLFW_KEY_LEFT_ALT])
		{
			b_Conditions[Conditions::Alt_Press] = true;
		}

		if (!pab_KeyArray_In[GLFW_KEY_LEFT_ALT])
		{
			b_Conditions[Conditions::Alt_Press] = false;
		}

		if (pab_KeyArray_In[GLFW_KEY_LEFT_CONTROL] && pab_KeyArray_In[GLFW_KEY_S] && !pab_LockedKeys_In[GLFW_KEY_S])
		{
			save->save(mspo_Objects, "assets/scenes/Robot_Scene.xml");
			pab_LockedKeys_In[GLFW_KEY_S] = true;
			std::cout << "Scene saved..." << "\n";
		}
		if (!pab_KeyArray_In[GLFW_KEY_S]) pab_LockedKeys_In[GLFW_KEY_S] = false;

		if (pab_KeyArray_In[GLFW_KEY_DELETE] && !pab_LockedKeys_In[GLFW_KEY_DELETE])
		{
			b_Conditions[Conditions::Delete] = true;
			pab_LockedKeys_In[GLFW_KEY_DELETE] = true;
		}
		if (!pab_KeyArray_In[GLFW_KEY_DELETE])
		{
			pab_LockedKeys_In[GLFW_KEY_DELETE] = false;
			b_Conditions[Conditions::Delete] = false;
		}
	}
}

void Scene_3D::mouse_Input(GLboolean* pab_MouseArray_In, GLboolean* pab_LockedMouse_In)
{
	if (pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT] && !pab_LockedMouse_In[GLFW_MOUSE_BUTTON_LEFT] && !b_Conditions[Conditions::Alt_Press])
	{
		b_Conditions[Conditions::LeftMouse_Button] = true;

		pab_LockedMouse_In[GLFW_MOUSE_BUTTON_LEFT] = true;
	}
	if (!pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT]) pab_LockedMouse_In[GLFW_MOUSE_BUTTON_LEFT] = false;

	if (pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT] && b_Conditions[Conditions::Alt_Press])
	{
		lock_mouse(true);
		b_Conditions[Conditions::Move_Mouse] = true;
	}

	if (pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT] && pickedID == 1000000 ||
		pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT] && pickedID == 1000001 ||
		pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT] && pickedID == 1000002)
	{
		lock_mouse(true);
		b_Conditions[Conditions::Stop_Arrow] = false;
	}

	if (!pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT] && pickedID == 1000000 ||
		!pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT] && pickedID == 1000001 ||
		!pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT] && pickedID == 1000002)
	{
		b_Conditions[Conditions::Stop_Arrow] = true;
	}

	if (pab_MouseArray_In[GLFW_MOUSE_BUTTON_RIGHT])
	{
		lock_mouse(true);
		b_Conditions[Conditions::RightMouse_Button] = true;
	}

	if (!pab_MouseArray_In[GLFW_MOUSE_BUTTON_RIGHT])
	{
		b_Conditions[Conditions::RightMouse_Button] = false;
	}

}

void Scene_3D::scroll_Input(glm::vec2 v2_Scroll_In)
{
	if (v2_Scroll_In.y > 0)
	{
		camera_3D->set_Speed(f_Speed / 10);
		camera_3D->move_Forward();
	}
	else
	{
		camera_3D->set_Speed(f_Speed / 10);
		camera_3D->move_Backward();
	}
}

void Scene_3D::reload_Scene()
{
	clean_Up();
	b_Init = false;
}

//Update the scene
void Scene_3D::update_Scene(GLfloat f_Delta_In, glm::vec2 v2_MousePos_In)
{
	v2_MousePos = v2_MousePos_In;
	////Initialize
	if (!b_Init) init();

	if (b_Init)
	{
		f_Speed = 2000 * f_Delta_In;

		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_ObjectID() == pickedID)
			{
				if (pair.second->get_Tag() != "Arrow" && pair.second->get_Tag() != "GUI")
				{
					picked_Object = static_cast<GameObject_3D*>(pair.second);

					if (picked_Object->get_Parent() != nullptr)
					{
						auto parent = static_cast<GameObject_3D*>(picked_Object->get_Parent());
						static_cast<GameObject_3D*>(mspo_Objects.find("Red Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
						static_cast<GameObject_3D*>(mspo_Objects.find("Green Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));
						static_cast<GameObject_3D*>(mspo_Objects.find("Blue Arrow")->second)->set_Position(glm::vec3(parent->get_Position()));

						static_cast<GameObject_3D*>(mspo_Objects.find("Red Arrow")->second)->set_RenderStatus(true);
						static_cast<GameObject_3D*>(mspo_Objects.find("Green Arrow")->second)->set_RenderStatus(true);
						static_cast<GameObject_3D*>(mspo_Objects.find("Blue Arrow")->second)->set_RenderStatus(true);
					}
					else if (picked_Object->get_Children().empty())
					{
						static_cast<GameObject_3D*>(mspo_Objects.find("Red Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
						static_cast<GameObject_3D*>(mspo_Objects.find("Green Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));
						static_cast<GameObject_3D*>(mspo_Objects.find("Blue Arrow")->second)->set_Position(glm::vec3(picked_Object->get_Position()));

						static_cast<GameObject_3D*>(mspo_Objects.find("Red Arrow")->second)->set_RenderStatus(true);
						static_cast<GameObject_3D*>(mspo_Objects.find("Green Arrow")->second)->set_RenderStatus(true);
						static_cast<GameObject_3D*>(mspo_Objects.find("Blue Arrow")->second)->set_RenderStatus(true);
					}
				}
				else if (pair.second->get_Tag() == "GUI")
				{
					process_2DClick(pickedID);
				}
				else
				{
					process_3DClick(f_Delta_In, v2_MousePos);

				}
			}

			pair.second->update();
			if (pair.second->get_Tag() == "Particle")
			{
				static_cast<GameObject_Instanced*>(pair.second)->update_Particles(f_Delta_In, 1.0f, 1.0f, glm::vec3(1.0f));
			}

		}
		if (b_Conditions[Conditions::RightMouse_Button]) camera_3D->move_MouseEditor(f_Delta_In, v2_MousePos_In, v2_WindowSize);

		camera_3D->move_Keyboard(f_Delta_In);
		if (b_Conditions[Conditions::Move_Mouse]) camera_3D->move_Mouse(f_Delta_In, v2_MousePos_In, v2_WindowSize);
		camera_3D->update();
		camera_3D->reset();
	}
}

//Render all scene objects
void Scene_3D::render()
{
	if (b_Init)
	{
		if (b_Conditions[Conditions::LeftMouse_Button])
		{
			render_ColourScene();
			if (b_Conditions[Conditions::Selection])
			{
				for (auto const& pair : mspo_Objects)
				{
					if (pair.second->get_Tag() == "GUI" &&  pair.second->get_ObjectType() == "Hidden_Show")
					{
						if (pair.first == "Show_PositionTextbox")
						{
							for (auto const find_obj : mspo_Objects)
							{
								if (find_obj.second->get_ObjectID() == pickedID && find_obj.second->get_Tag() != "GUI" && find_obj.second->get_Tag() != "Arrow")
								{
									auto object = static_cast<GameObject_3D*>(find_obj.second);

									if (object->get_Parent() != nullptr)
									{
										auto parent = static_cast<GameObject_3D*>(object->get_Parent());
										static_cast<Textbox*>(pair.second)->set_Text(to3DVectorString(parent->get_Position()));
									}
									else static_cast<Textbox*>(pair.second)->set_Text(to3DVectorString(object->get_Position()));

								}
							}
						}
						else if (pair.first == "Show_OriginTextbox")
						{
							for (auto const find_obj : mspo_Objects)
							{
								if (find_obj.second->get_ObjectID() == pickedID && find_obj.second->get_Tag() != "GUI" && find_obj.second->get_Tag() != "Arrow")
								{
									auto object = static_cast<GameObject_3D*>(find_obj.second);

									if (object->get_Parent() != nullptr)
									{
										auto parent = static_cast<GameObject_3D*>(object->get_Parent());
										static_cast<Textbox*>(pair.second)->set_Text(to3DVectorString(parent->get_Origin()));
									}
									else static_cast<Textbox*>(pair.second)->set_Text(to3DVectorString(object->get_Origin()));
								}
							}
						}
						else if (pair.first == "Show_ScaleTextbox")
						{
							for (auto const find_obj : mspo_Objects)
							{
								if (find_obj.second->get_ObjectID() == pickedID && find_obj.second->get_Tag() != "GUI" && find_obj.second->get_Tag() != "Arrow")
								{
									auto object = static_cast<GameObject_3D*>(find_obj.second);

									if (object->get_Parent() != nullptr)
									{
										auto parent = static_cast<GameObject_3D*>(object->get_Parent());
										static_cast<Textbox*>(pair.second)->set_Text(to3DVectorString(parent->get_Scale()));
									}
									else static_cast<Textbox*>(pair.second)->set_Text(to3DVectorString(object->get_Scale()));
								}
							}
						}
						else if (pair.first == "Show_ObjectTextbox")
						{
							for (auto const find_obj : mspo_Objects)
							{
								if (find_obj.second->get_ObjectID() == pickedID && find_obj.second->get_Tag() != "GUI" && find_obj.second->get_Tag() != "Arrow")
								{
									auto object = static_cast<GameObject_3D*>(find_obj.second);
									if (object->get_Parent() != nullptr)
									{
										auto parent = static_cast<GameObject_3D*>(object->get_Parent());
										static_cast<Textbox*>(pair.second)->set_Text(parent->get_Name());
									}
									else static_cast<Textbox*>(pair.second)->set_Text(object->get_Name());
								}
							}
						}
						else if (pair.first == "Show_PrefabTextbox")
						{
							for (auto const find_obj : mspo_Objects)
							{
								if (find_obj.second->get_ObjectID() == pickedID && find_obj.second->get_Tag() != "GUI" && find_obj.second->get_Tag() != "Arrow")
								{
									auto object = static_cast<GameObject_3D*>(find_obj.second);
									if (object->get_Parent() != nullptr)
									{
										auto parent = static_cast<GameObject_3D*>(object->get_Parent());
										static_cast<Textbox*>(pair.second)->set_Text(parent->get_PrefabName());
									}
									else static_cast<Textbox*>(pair.second)->set_Text(object->get_PrefabName());
								}
							}
						}
						else if (pair.first == "Show_IDTextbox")
						{
							for (auto const find_obj : mspo_Objects)
							{
								if (find_obj.second->get_ObjectID() == pickedID && find_obj.second->get_Tag() == "Light")
								{
									auto object = static_cast<GameObject_3D*>(find_obj.second);
									if (object->get_Parent() != nullptr)
									{
										auto parent = static_cast<GameObject_3D*>(object->get_Parent());
										static_cast<Textbox*>(pair.second)->set_Text(std::to_string(static_cast<Point_Light*>(pair.second)->get_ID()));
									}
									else static_cast<Textbox*>(pair.second)->set_Text(std::to_string(static_cast<Point_Light*>(find_obj.second)->get_ID()));
								}
							}
						}
					}
				}
			}
		}

		glEnable(GL_BLEND);
		glUseProgram(po_Loader->get_Shader("0")->get_Program());
		unsigned int tex_No = 0;
		unsigned int light_No = 0;

		float d[100];
		std::string b_Shadow;
		GLint b_Shadow_Loc;
		std::string ui_Shadow;
		GLint ui_Shadow_Loc;
		int l_iter = 0;
		ui_light_Amount = 0;
		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "Light")
			{
				light[static_cast<Point_Light*>(pair.second)->get_ID()] = static_cast<Point_Light*>(pair.second)->get_Position();
				ui_light_Amount++;
				l_iter++;
			}
		}

		for (unsigned int i = 0; i < ui_light_Amount; i++)
		{
			d[i] = glm::distance(camera_3D->get_CameraPos(), light[i]);
			b_Shadow = "point_Light[" + std::to_string(i) + "].casts_Shadow";
			b_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), b_Shadow.c_str());
			glUniform1i(b_Shadow_Loc, false);

			ui_Shadow = "point_Light[" + std::to_string(i) + "].ui_depth_Map";
			ui_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), ui_Shadow.c_str());
			glUniform1i(ui_Shadow_Loc, 9);
		}

		unsigned int light_Nom[3];
		light_Nom[0] = 0;
		float closest_Distance = 9999.0f;
		for (unsigned int i = 0; i < ui_light_Amount; i++)
		{
			if (d[i] < closest_Distance)
			{
				closest_Distance = d[i];
				light_Nom[0] = i;
			}
		}

		b_Shadow = "point_Light[" + std::to_string(light_Nom[0]) + "].casts_Shadow";
		b_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), b_Shadow.c_str());
		glUniform1i(b_Shadow_Loc, true);
		ui_Shadow = "point_Light[" + std::to_string(light_Nom[0]) + "].ui_depth_Map";
		ui_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), ui_Shadow.c_str());
		glUniform1i(ui_Shadow_Loc, 0);
		float closest_Distance2 = 9999.0f;
		for (unsigned int i = 0; i < ui_light_Amount; i++)
		{
			if (d[i] < closest_Distance2 && d[i] > closest_Distance)
			{
				closest_Distance2 = d[i];
				light_Nom[1] = i;
			}
		}

		b_Shadow = "point_Light[" + std::to_string(light_Nom[1]) + "].casts_Shadow";
		b_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), b_Shadow.c_str());
		glUniform1i(b_Shadow_Loc, true);

		ui_Shadow = "point_Light[" + std::to_string(light_Nom[1]) + "].ui_depth_Map";
		ui_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), ui_Shadow.c_str());
		glUniform1i(ui_Shadow_Loc, 1);
		float closest_Distance3 = 9999.0f;
		for (unsigned int i = 0; i < ui_light_Amount; i++)
		{
			if (d[i] < closest_Distance3 && d[i] > closest_Distance2)
			{
				closest_Distance3 = d[i];
				light_Nom[2] = i;
			}
		}

		b_Shadow = "point_Light[" + std::to_string(light_Nom[2]) + "].casts_Shadow";
		b_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), b_Shadow.c_str());
		glUniform1i(b_Shadow_Loc, true);


		ui_Shadow = "point_Light[" + std::to_string(light_Nom[2]) + "].ui_depth_Map";
		ui_Shadow_Loc = glGetUniformLocation(po_Loader->get_Shader("0")->get_Program(), ui_Shadow.c_str());
		glUniform1i(ui_Shadow_Loc, 2);

		glUseProgram(po_Loader->get_Shader("6")->get_Program());

		for (int i = 0; i < 3; i++)
		{
			unsigned no = light_Nom[i];

			o_SceneLoader->prepare_DepthCube(po_Loader->get_Shader("6"), light[no], depth[i], i);


			for (auto const& pair : mspo_Objects)
			{


				if (pair.second->get_Tag() != "Object_Lamp" &&  pair.second->get_Tag() != "Particle" && pair.second->get_Tag() != "Light")
				{

					pair.second->renderDepth(po_Loader->get_Shader("6"));
				}


			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glViewport(0, 0, v2_WindowSize.x, v2_WindowSize.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(po_Loader->get_Shader("0")->get_Program());
		camera_3D->update_Shader(po_Loader->get_Shader("0"));
		glUseProgram(po_Loader->get_Shader("7")->get_Program());
		camera_3D->update_Shader(po_Loader->get_Shader("7"));

		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "Light")
			{
				static_cast<Light*>(pair.second)->update_Shader(po_Loader->get_Shader("0"));
				pair.second->render(po_Loader->get_Shader("0"));
			}
			else if (pair.second->get_Tag() == "Particle")
			{
				glUseProgram(po_Loader->get_Shader("7")->get_Program());
				pair.second->render(po_Loader->get_Shader("7"));
			}
			else if (pair.second->get_Tag() == "GUI")
			{
			}
			else
			{
				glUseProgram(po_Loader->get_Shader("0")->get_Program());
				pair.second->render(po_Loader->get_Shader("0"));
			}
		}
	}

	glDisable(GL_BLEND);

	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(po_Loader->get_Shader("5")->get_Program());
	camera_3D->update_Shader(po_Loader->get_Shader("5"));
	for (auto const& pair : mspo_Objects) if (pair.second->get_Tag() == "Arrow") pair.second->render(po_Loader->get_Shader("5"));

	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(po_Loader->get_Shader("1")->get_Program());
	camera_2D->update_Shader(po_Loader->get_Shader("1"));
	for (auto const& pair : mspo_Objects)
	{
		if (pair.second->get_Tag() == "GUI")
		{
			pair.second->render(po_Loader->get_Shader("1"));
		}

	}

	lock_mouse(false);
	b_Conditions[Conditions::Move_Mouse] = false;
	b_Conditions[Conditions::LeftMouse_Button] = false;
	b_Conditions[Conditions::ShiftDetected] = false;

	if (b_Conditions[Conditions::Delete])
	{
		std::vector<std::string> s_Deletion;

		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_Tag() == "Object" && pair.second->get_ObjectID() == pickedID ||
				pair.second->get_Tag() == "Object_NonSavable" && pair.second->get_ObjectID() == pickedID)
			{
				//Lots to do here
				if (pair.second->get_Parent() != nullptr)
				{
					for (auto const& children : pair.second->get_Children()) s_Deletion.push_back(children.first);
					s_Deletion.push_back(pair.first);
				}
				else s_Deletion.push_back(pair.first);
			}
		}

		if (!s_Deletion.empty())
		{
			//Perform deletion here
			for (unsigned int i = 0; i < s_Deletion.size(); i++)
			{
				auto found = mspo_Objects.find(s_Deletion.at(i))->second;
				found->clean_Up();
				delete found;
				mspo_Objects.erase(s_Deletion.at(i));
			}
		}
	}

	if (b_Conditions[Conditions::ReloadScene])
	{
		clean_Up();
		b_Init = false;
		b_Conditions[Conditions::ReloadScene] = false;
		pickedID = -1;
	}
}

void Scene_3D::clean_Up()
{
	if (!mspo_Objects.empty())
	{
		//For each object
		for (auto const& pair : mspo_Objects)
		{
			pair.second->clean_Up();
			delete pair.second;
		}
		mspo_Objects.clear();
	}

	pickedID = -1;
	delete o_SceneLoader;
	delete camera_3D;
	delete save;
}