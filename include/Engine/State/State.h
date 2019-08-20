#pragma once

#include <vector>

class Scene;

class State
{
private:
	//States
	std::vector<Scene*> vpo_GameStates;

public:
	//Constructor
	State() {};

	//Last state
	Scene* back();
	void push_State(Scene* po_Scene_In); // Push the state
	void pop_State(); // Pop the state
	bool is_Empty(); // Is stack empty?
};