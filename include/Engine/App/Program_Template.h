#pragma once

#include <glad/glad.h>
#include <glm\glm.hpp>
#include <Engine/State/State.h>

class Scene;
class Loader;

class Program_Template
{
protected:
	Loader* po_Loader; // Resource loader
	State o_State; // State
	GLboolean ab_Keys[1024]; // Keys
	GLboolean ab_LockedKeys[1024]; // Non repeatable keys
	GLboolean ab_MousePress[1024]; // Mouse presses
	GLboolean ab_LockedMouse[1024]; // Mouse lock
	bool b_GameIsOver; // Is game over?
	bool b_LockMouse; // Is mouse locked?
	glm::vec2 v2_WindowSize;
	int i_KeyPress;
public:

	//Initialzie
	virtual void init() = 0;

	//Process inputs
	virtual void process_Input(GLfloat f_Delta_In) = 0;

	//Update the game
	virtual void update(GLfloat f_Delta_In, glm::vec2 vec2_MousePos_In) = 0;

	//Draw anything on the screen
	virtual void draw() = 0;

	//Process inputs
	virtual void process_Key(int i_Key_In, GLboolean b_State_In) = 0;
	virtual void process_Mouse(int i_Button_In, GLboolean b_State_In) = 0;
	virtual void process_Scroll(glm::vec2 v2_Scroll_In) = 0;

	//Clean up
	virtual void clean_Up() = 0;

	//Is the game over?
	bool is_GameOver();

	//Lock mouse
	void mouse_Lock(bool b_Cond_In);

	//Is mouse locked?
	bool is_MouseLocked();

	void set_WindowSize(glm::vec2 v2_WindowSize_In);

};