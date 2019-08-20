#pragma once

#include <Engine\Scene\Scene.h>
#include <Engine\Loaders\SceneSaver.h>
#include <Engine\Game_Objects\Camera_2D.h>
#include <Engine\Game_Objects\GameObject_2D.h>

class Mesh_2D;
namespace Conditions_2D
{
	enum Conditions{Render_Basics, LeftMouse, ListenToKeyboard, ShiftDetected, Num};
}

class Scene_2D : public Scene
{
private:
	float f_Speed;
	std::map<std::string, int> msio_ObjectIDs;
	int pickedID;
	Camera_2D* camera_2D;
	GameObject_2D* picked_Object;	
	glm::vec2 v2_MousePos;

	int i_Counter;

	bool b_Conditions[Conditions_2D::Num];
	void render_ColourScene();

	int i_KeyPress;

public:
	//Constructor
	Scene_2D() {};

	//Initialize
	void init();

	//Do something with keyboard input
	void keyboard_Input(GLfloat f_Delta_In, GLboolean* pab_KeyArray_In, GLboolean* pab_LockedKeys_In, int i_KeyPress);
	void mouse_Input(GLboolean* pab_MouseArray_In, GLboolean* pab_LockedMouse_In);
	void scroll_Input(glm::vec2 v2_Scroll_In);
	//Update the scene
	void update_Scene(GLfloat f_Delta_In, glm::vec2 v2_MousePos_In);

	//Render all scene objects
	void render();

	void clean_Up();

	void reload_Scene();
};