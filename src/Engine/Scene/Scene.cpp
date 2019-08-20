#include <Engine/Scene/Scene.h>

//Pass loader
void Scene::pass_Loader(Loader* po_Loader_In)
{
	po_Loader = po_Loader_In;
}

// Pass Audio
void Scene::pass_Audio(std::map<std::string, Sound*>* po_Sound_In)
{
	//snd_Audio = po_Sound_In;
}

//Is loader empty?
bool Scene::is_LoaderEmpty()
{
	if (po_Loader == nullptr) return true;
	return false;
}

//Lock mouse
void Scene::lock_mouse(bool b_Cond_In)
{
	b_LockMouse = b_Cond_In;
}

//Is mouse locked?
bool Scene::is_MouseLocked()
{
	return b_LockMouse;
}

void Scene::set_WindowSize(glm::vec2 v2_WindowSize_In)
{
	v2_WindowSize = v2_WindowSize_In;
}

//Extract state
void Scene::extract_State(State* po_State_In)
{
	po_GameState = po_State_In;
}