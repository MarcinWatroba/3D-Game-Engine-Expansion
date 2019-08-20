#include "Game_Object.h"

void Game_Object::set_RenderStatus(bool b_RenderStatus_In)
{
}

void Game_Object::set_Container()
{
}

bool Game_Object::is_Container()
{
	return false;
}

void Game_Object::add_Child(Game_Object * po_Child_In)
{
}

std::map<std::string, Game_Object*> Game_Object::get_Children()
{
	return std::map<std::string, Game_Object*>();
}

void Game_Object::set_Parent(Game_Object * po_Parent_In)
{
}

Game_Object * Game_Object::get_Parent()
{
	return nullptr;
}
