#pragma once
#include <Engine\Stats\Stats.h>
#include <map>
#include <TinyXML2\tinyxml2.h>
#include <iostream>
#include <glm\glm.hpp>

class StatsLoader
{
private:
	std::map<std::string, Stats*> statsList;
	std::vector<glm::vec3> loadPath(tinyxml2::XMLDocument* po_doc, std::string pc_PathName_In);
public:
	StatsLoader(const char * pc_FileName_In);
	~StatsLoader();
	Stats* get_Stat(std::string temp);
};