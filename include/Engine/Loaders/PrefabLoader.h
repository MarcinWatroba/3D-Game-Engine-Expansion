#pragma once

#include <Engine\Game_Objects\GameObject_3D.h>
#include <Engine\Game_Objects\GameObject_Instanced.h>
#include <map>

class Loader;
class StatsLoader;
class Stats;

class PrefabLoader
{
private:
	Loader* po_SceneLoader;
	StatsLoader* po_StatsLoader;
	std::map <std::string, Game_Object*> mipo_Prefabs;

	glm::vec3 to3DVector(const char * pc_Vector3D_In);
	glm::vec2 to2DVector(const char * pc_Vector2D_In);
	glm::quat toQuat(const char* pc_Quaternion_In);

	//Add children
	void add_Components(GameObject_3D* po_GameObject_In, std::string s_ToProcess_In, Stats * stats_In);

	//Identify the component
	void identify_Component(GameObject_3D* po_GameObject_In, std::string& s_Result_In, Stats * stat_In);
public:
	PrefabLoader() {};
	PrefabLoader(const char* pc_FileName_In, Loader* po_Loader_In, StatsLoader* po_StatsLoader_In);

	void add_Children(std::vector<std::string>& vs_Children_In, std::string s_ToProcess_In);

	//Get font
	Game_Object* get_Prefab(std::string s_Name_In);

	std::map <std::string, Game_Object*>& get_PrefabMap();

	//Clean up the memory
	void clean_Up();
};