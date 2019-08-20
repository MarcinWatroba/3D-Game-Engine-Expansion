#include "Engine\Game_Objects\Game_Object.h"


Game_Object::Game_Object() :
	b_toDelete(false),
	po_Parent(nullptr)
{}

bool Game_Object::get_ToDelete()
{
	return b_toDelete;
}

void Game_Object::set_ToDelete()
{
	//first set only highest level parent to be deleted
	if (po_Parent != nullptr && !po_Parent->get_ToDelete())
	{
		po_Parent->set_ToDelete();
	}
	else
	{
		//if highest level parent or parent has been set to be deleted
		b_toDelete = true;

		//set all children of object to be destroyed too
		if (is_Container())
		{
			for (auto const& children : get_Children())
			{
				children.second->set_ToDelete();
			}
		}
	}
}

glm::mat4 Game_Object::get_ParentMatrix() { return glm::mat4(1.0f); }

void Game_Object::set_RenderStatus(bool b_RenderStatus_In)
{
	b_RenderStatus = b_RenderStatus_In;
}

bool Game_Object::get_RenderStatus()
{
	return b_RenderStatus;
}

void Game_Object::set_Prefab(std::string s_Name_In)
{
	s_PrefabName = s_Name_In;
}

std::string Game_Object::get_PrefabName()
{
	return s_PrefabName;
}

void Game_Object::add_Child(Game_Object* po_Child_In)
{
	b_IsContainer = true;
	po_Child_In->set_Parent(this);
	mspo_Children.insert(std::pair<std::string, Game_Object*>(po_Child_In->get_Name(), po_Child_In));
}

void Game_Object::set_Parent(Game_Object* po_Parent_In)
{
	po_Parent = po_Parent_In;
}

Game_Object* Game_Object::get_Parent()
{
	return po_Parent;
}

std::string Game_Object::get_Name()
{
	return s_Name;
}

void Game_Object::add_ChildName(std::string s_Name_In)
{
	vs_ChildrenNames.push_back(s_Name_In);
}

std::vector<std::string>& Game_Object::get_ChildrenNames()
{
	return vs_ChildrenNames;
}

void Game_Object::set_Name(std::string s_Name_In)
{
	s_Name = s_Name_In;
}

void Game_Object::set_ObjectType(std::string s_Type_In)
{
	s_ObjectType = s_Type_In;
}

std::map<std::string, Component*>& Game_Object::get_Components()
{
	return mipo_Components;
}

Component* Game_Object::get_Component(std::string s_Name_In)
{
	std::map<std::string, Component*>::iterator iter = mipo_Components.find(s_Name_In);
	if (iter != mipo_Components.end()) {
		return iter->second;
	}
	//not found
	return nullptr;
}

std::map<std::string, Game_Object*>& Game_Object::get_Children()
{
	return mspo_Children;
}

void Game_Object::set_Tag(std::string s_Tag_In)
{
	s_Tag = s_Tag_In;
}



std::string Game_Object::get_Tag()
{
	return s_Tag;
}

bool Game_Object::is_Container()
{
	return b_IsContainer;
}

std::string Game_Object::get_ObjectType()
{
	return s_ObjectType;
}

void Game_Object::set_ObjectID(int i_ID_In)
{
	i_ObjectID = i_ID_In;
}

int Game_Object::get_ObjectID()
{
	return i_ObjectID;
}

Game_Object::Game_Object(const Game_Object &p_NewObject_In)
{
	s_Tag = p_NewObject_In.s_Tag;
	po_Parent = p_NewObject_In.po_Parent;
	b_IsContainer = p_NewObject_In.b_IsContainer;
	i_ObjectID = p_NewObject_In.i_ObjectID;
	b_RenderStatus = p_NewObject_In.b_RenderStatus;
	v2_Tiling = p_NewObject_In.v2_Tiling;
}
