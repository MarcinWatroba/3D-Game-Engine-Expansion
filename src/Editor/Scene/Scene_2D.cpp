#include <Editor\Scene\Scene_2D.h>

#include <GLFW\glfw3.h>
#include <Engine/Mesh/Mesh_2D.h>
#include <Engine\Game_Objects\GameObject_2D.h>
#include <Engine/Component/RenderComp_2D.h>
#include <Engine\Component\Transform_2D.h>
#include <iostream>
#include <Engine\GUI\Font.h>
#include <Engine\GUI\Glyph.h>
#include <Engine\GUI\Letter.h>
#include <Engine\GUI\Text.h>
#include <Engine\GUI\Button.h>
#include <Engine\GUI\Tile.h>
#include <Engine\GUI\Textbox.h>
#include <algorithm>

void Scene_2D::render_ColourScene()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(po_Loader->get_Shader("4")->get_Program());
	camera_2D->update_Shader(po_Loader->get_Shader("4"));

	for (auto const& pair : mspo_Objects)
	{
		int r = (pair.second->get_ObjectID() & 0x000000FF) >> 0;
		int g = (pair.second->get_ObjectID() & 0x0000FF00) >> 8;
		int b = (pair.second->get_ObjectID() & 0x00FF0000) >> 16;

		GLuint pickingColourID = glGetUniformLocation(po_Loader->get_Shader("4")->get_Program(), "pickColour");
		glUniform4f(pickingColourID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

		pair.second->render(po_Loader->get_Shader("4"));
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

//Initialize everything once
void Scene_2D::init()
{
	b_Init = false;
	b_Conditions[0] = false;
	pickedID = -1;
	camera_2D = new Camera_2D(0, v2_WindowSize.x, v2_WindowSize.y, 0);
	float scale = v2_WindowSize.x / v2_WindowSize.y;

	//creator->add_Button(new Button(0, "Add GUI Element", po_Loader->get_Font("0"), po_Loader->get_Mesh("0"), po_Loader->get_Texture("8"), glm::vec2(0, v2_WindowSize.y - 50.f), glm::vec2(200.f, 50.f), 0.f, 0.f));

	//mspo_Objects.insert(std::pair<std::string, Game_Object*>("Element_Button", new Button(0, "Add GUI Element", po_Loader->get_Font("0"), po_Loader->get_Mesh("0"), po_Loader->get_Texture("8"), glm::vec2(0, v2_WindowSize.y - 50.f), glm::vec2(200.f, 50.f), 0.f, 0.f)));
	//mspo_Objects.insert(std::pair<std::string, Game_Object*>("Tile", new Tile(1, "Tile", po_Loader->get_Mesh("1"), po_Loader->get_Texture("8"), glm::vec2(0.f, v2_WindowSize.y - 100.f), glm::vec2(v2_WindowSize.x, 100.f), 0.f, 0.f)));
	//mspo_Objects.insert(std::pair<std::string, Game_Object*>("Data_Textbox", new Textbox(2, po_Loader->get_Font("0"), po_Loader->get_Mesh("0"), po_Loader->get_Texture("8"), v2_WindowSize / 2.f, glm::vec2(0.f, 0.f), 0.f, 0.f)));
	//mspo_Objects.insert(std::pair<std::string, Game_Object*>("Generic_Text", new Text(3, "", glm::vec2(0.f), 0, po_Loader->get_Font("0"), 0.f)));
	//mspo_Objects.insert(std::pair<std::string, Game_Object*>("Ok", new Button(0, "OK", po_Loader->get_Font("0"), po_Loader->get_Mesh("0"), po_Loader->get_Texture("8"), glm::vec2(0, v2_WindowSize.y - 50.f), glm::vec2(200.f, 50.f), 0.f, 0.f)));
	//mspo_Objects.insert(std::pair<std::string, Game_Object*>("Cancel", new Button(0, "Cancel", po_Loader->get_Font("0"), po_Loader->get_Mesh("0"), po_Loader->get_Texture("8"), glm::vec2(0, v2_WindowSize.y - 50.f), glm::vec2(200.f, 50.f), 0.f, 0.f)));

	b_Init = true;

	i_KeyPress = 0;
}
//Do something with keyboard input
void Scene_2D::keyboard_Input(GLfloat f_Delta_In, GLboolean* pab_KeyArray_In, GLboolean* pab_LockedKeys_In, int i_KeyPress_In)
{

	if (b_Conditions[Conditions_2D::ListenToKeyboard])
	{
		if (pab_KeyArray_In[GLFW_KEY_LEFT_SHIFT] && !pab_LockedKeys_In[GLFW_KEY_LEFT_SHIFT])
		{
			b_Conditions[Conditions_2D::ShiftDetected] = true;
		}

		if (pab_KeyArray_In[i_KeyPress_In] && !pab_LockedKeys_In[i_KeyPress_In] && i_KeyPress_In != GLFW_KEY_LEFT_SHIFT)
		{
			pab_LockedKeys_In[i_KeyPress_In] = true;
			//std::cout << i_KeyPress_In << "\n";

			for (auto const& pair : mspo_Objects)
			{
				if (pair.second->get_ObjectID() == pickedID && pair.second->get_Tag() == "Textbox") // We got it
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
						text_Box->insert_NewLine();
					}
					else
					{
						std::string s_Letter;

						s_Letter.push_back(i_KeyPress_In);
						if (!b_Conditions[Conditions_2D::ShiftDetected]) std::transform(s_Letter.begin(), s_Letter.end(), s_Letter.begin(), ::tolower);				
						s_Letter.push_back(i_KeyPress_In);
									
						text_Box->add_Letter(s_Letter);
					}
				}
			}

		}
		if (!pab_KeyArray_In[i_KeyPress_In]) pab_LockedKeys_In[i_KeyPress_In] = false;
	}
	else
	{
		if (pab_KeyArray_In[GLFW_KEY_SPACE] && !pab_LockedKeys_In[GLFW_KEY_SPACE])
		{
			b_Conditions[Conditions_2D::Render_Basics] = !b_Conditions[Conditions_2D::Render_Basics];
			pab_LockedKeys_In[GLFW_KEY_SPACE] = true;
		}
		if (!pab_KeyArray_In[GLFW_KEY_SPACE]) pab_LockedKeys_In[GLFW_KEY_SPACE] = false;
	}
}

void Scene_2D::mouse_Input(GLboolean* pab_MouseArray_In, GLboolean* pab_LockedMouse_In)
{
	if (pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT] && !pab_LockedMouse_In[GLFW_MOUSE_BUTTON_LEFT])
	{
		b_Conditions[Conditions_2D::LeftMouse] = true;
		pab_LockedMouse_In[GLFW_MOUSE_BUTTON_LEFT] = true;
	}
	if (!pab_MouseArray_In[GLFW_MOUSE_BUTTON_LEFT]) pab_LockedMouse_In[GLFW_MOUSE_BUTTON_LEFT] = false;

}

void Scene_2D::scroll_Input(glm::vec2 v2_Scroll_In)
{

}

void Scene_2D::reload_Scene()
{
	clean_Up();
	b_Init = false;
}

//Update the scene
void Scene_2D::update_Scene(GLfloat f_Delta_In, glm::vec2 v2_MousePos_In)
{
	v2_MousePos = v2_MousePos_In;

	if (!b_Init) init();

	if (b_Init)
	{
		

		for (auto const& pair : mspo_Objects)
		{
			//if (pair.second->get_ObjectID() == pickedID && pair.second->get_Tag() == "Textbox")
			//{
			//	b_Conditions[Conditions_2D::ListenToKeyboard] = true;
			//}
			//else
			//{
			//	b_Conditions[Conditions_2D::ListenToKeyboard] = false;
			//}

			//if (pair.second->get_ObjectID() == pickedID && pair.first == "Element_Button")
			//{
			//	b_Conditions[Conditions_2D::ListenToKeyboard] = true;

			//	auto gen_Text = static_cast<Text*>(mspo_Objects.find("Generic_Text")->second);
			//	auto data_Box = static_cast<Textbox*>(mspo_Objects.find("Data_Textbox")->second);
			//	auto ok_Button = static_cast<Button*>(mspo_Objects.find("OK")->second);

			//	gen_Text->set_Position(glm::vec2(v2_WindowSize.x / 2.f, v2_WindowSize.y / 2.f - 50));
			//	gen_Text->set_Text("Enter the name of the GUI type you want to add:");
			//	data_Box->set_Position(gen_Text->get_Position() + 50.f);
			//	data_Box->set_Size(glm::vec2(300.f, 20.f));
			//	ok_Button->set_Position(glm::vec2(v2_WindowSize.x / 2.f, v2_WindowSize.y / 2.f - 50));
			//}
			



			pair.second->update();
		}
	}
}

//Render all scene objects
void Scene_2D::render()
{
	if (b_Init)
	{
		if (b_Conditions[Conditions_2D::LeftMouse]) render_ColourScene();

		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glUseProgram(po_Loader->get_Shader("1")->get_Program());
		camera_2D->update_Shader(po_Loader->get_Shader("1"));
		for (auto const& pair : mspo_Objects)
		{
			if (pair.second->get_ObjectID() == pickedID && pair.second->get_Tag() == "Textbox") b_Conditions[Conditions_2D::ListenToKeyboard] = true;
			pair.second->render(po_Loader->get_Shader("1"));	
		}
	}

	b_Conditions[Conditions_2D::LeftMouse] = false;
	b_Conditions[Conditions_2D::ShiftDetected] = false;
}

void Scene_2D::clean_Up()
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
}