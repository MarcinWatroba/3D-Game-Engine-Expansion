#pragma once
#include <map>

class Game_Object;

class SceneSaver
{
private:

public:
	SceneSaver() {};
	void save(std::map<std::string, Game_Object*>& mspo_GameObjects_In, std::string s_SceneName_In);
};