#include <Engine/State/State.h>
#include <Engine/Scene/Scene.h>
#include <iostream>

void State::push_State(Scene* po_Scene_In)
{
	std::cout << "Adding new scene..." << "\n";
	po_Scene_In->extract_State(this);
	vpo_GameStates.push_back(po_Scene_In);
}

void State::pop_State()
{
	if (!is_Empty())
	{
		std::cout << "Popping the current scene..." << "\n";
		back()->clean_Up();
		delete back();
		vpo_GameStates.pop_back();
	}
}

Scene* State::back()
{
	return vpo_GameStates.back();
}

bool State::is_Empty()
{
	return vpo_GameStates.empty();
}