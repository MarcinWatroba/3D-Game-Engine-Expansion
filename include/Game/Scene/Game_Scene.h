#pragma once

#include <Engine\Scene\Scene.h>
#include <Engine\Physics\CollisionManager.h>
#include <Engine/Component/RenderQuad.h>
#include <vector>
//#include <Label.h>
//#include <Directional_Light.h>
//#include <Point_Light.h>

namespace render
{
	enum Render_ID
	{
		Alpha,
		Num
	};
}

class Game_Scene : public Scene
{
private:
	//Label* o_Label;
	bool rendered;
	bool b_Render[render::Num];
	bool b_SwitchCamMode;
	bool shooting = false;
	bool firstTime = true;
	//Matt's Stuff
	int walkRate = 35;
	int walkCount = 0;
	int triggerHoldCount = 0;	// used in firing
	int ammoRemaining = 6;
	int firerate = 100;
	unsigned int currentLevel;

	//Collision Manager
	CollisionManager colManage;
	std::vector<std::string> levelList;

	//remove a game object from the scene
	void destroyGameObject(Game_Object* po_object);

	//find the player and set player pointer, returns false if no player found in scene
	bool findPlayer();
	GameObject_3D* player;
	RenderQuad quad;

	//number of enemies
	int i_numEnemies;

	//lighting
	glm::vec3 light[30];
	glm::vec3 pos[100];
	glm::uvec2 depth[30];
	float radius[30];
public:
	//Constructor
	Game_Scene();

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

	void load_Scene(int i);
};