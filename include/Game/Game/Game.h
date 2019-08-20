#pragma once
#include <Engine/App/Program_Template.h>
#include <Engine\Audio\Sound.h>

#include <map>

class AudioEngine;

class Game : public Program_Template
{
protected:
	AudioEngine* Audio_Engine;
	std::map<std::string, Sound*> snd_Audio;
	
public:
	//Constructor
	Game();

	//Initialize
	void init();

	//Process inputs
	void process_Input(GLfloat f_Delta_In);

	//Update the game
	void update(GLfloat f_Delta_In, glm::vec2 v2_MousePos_In);

	//Draw anything on the screen
	void draw();

	//Receive the key
	void process_Key(int i_KeyCode_In, GLboolean b_State_In);
	void process_Mouse(int i_Button_In, GLboolean b_State_In);
	void process_Scroll(glm::vec2 v2_Scroll_In);

	//Clean up everything
	void clean_Up();
};