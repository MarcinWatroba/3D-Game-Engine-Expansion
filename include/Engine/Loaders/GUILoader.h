#pragma once

#include <map>

class PrefabLoader;
class Game_Object;
class GameObject_2D;

class GUILoader
{
private:
	glm::vec2 v2_WindowSize;

public:
	GUILoader() {};
	GUILoader(const char* pc_FileName_In, PrefabLoader* po_PrefLoader_In, std::map<std::string, Game_Object*>& mspo_GameObjects_In, glm::vec2 v2_WindowSize_In);
	glm::vec2 to2DVector(const char * pc_Vector2D_In);

	glm::vec2 apply_Pattern(GameObject_2D* p_Object_In, std::string s_WindowMode_In, glm::vec2 v2_Position_In);
};