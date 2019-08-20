#include <Engine\Loaders\StatsLoader.h>

StatsLoader::StatsLoader(const char * pc_FileName_In)
{
	tinyxml2::XMLDocument stats_File;
	stats_File.LoadFile(pc_FileName_In);
	tinyxml2::XMLElement* body = stats_File.FirstChildElement("stats");


	for (tinyxml2::XMLElement* it = body->FirstChildElement("new_Stat"); it != nullptr; it = it->NextSiblingElement("new_Stat"))
	{
		std::cout << "Adding new object to the scene..." << "\n";

		std::string s_ObjectName = it->Attribute("name");

		std::string s_Health = it->Attribute("health");
		float i_Health = std::strtof(s_Health.c_str(), nullptr);

		std::string s_Ammo = it->Attribute("ammo");
		float i_Ammo = std::strtof(s_Ammo.c_str(), nullptr);

		std::string s_Mass = it->Attribute("mass");
		float i_Mass = std::strtof(s_Mass.c_str(), nullptr);

		std::string s_Gravity = it->Attribute("gravity");
		bool b_Gravity = (s_Gravity == "true");

		std::string s_Path = it->Attribute("path");
		std::vector<glm::vec3> path;
		if (s_Path != "") { path = loadPath(&stats_File, s_Path.c_str()); }

		//set up stats class
		Stats * stats = new Stats(i_Health, i_Ammo, i_Mass, b_Gravity);
		for (unsigned int i = 0; i < path.size(); i++)
		{
			stats->addPathPoint(path[i]);
		}

		//add to list
		statsList.insert(std::make_pair(s_ObjectName, stats));
	}
}

std::vector<glm::vec3> StatsLoader::loadPath(tinyxml2::XMLDocument* po_doc, std::string s_PathName_In)
{
	std::string s_point;
	std::string s_val;
	unsigned int pos, pos2;
	std::vector<glm::vec3> path;
	tinyxml2::XMLElement* body = po_doc->FirstChildElement("patrols");

	//
	for (tinyxml2::XMLElement* it = body->FirstChildElement("patrolPath"); it != nullptr; it = it->NextSiblingElement("patrolPath"))
	{
		//find a patrol path with the input name
		std::string s_PathName = it->Attribute("name");
		if (s_PathName == s_PathName_In)
		{
			//get points
			for (tinyxml2::XMLElement* it2 = it->FirstChildElement("point"); it2 != nullptr; it2 = it2->NextSiblingElement("point"))
			{
				glm::vec3 point;
				s_point = it2->Attribute("pos");
				//
				pos = s_point.find(',');
				s_val = s_point.substr(0, pos++);
				point.x = std::strtof(s_val.c_str(), nullptr);
				//
				pos2 = s_point.find(',', ++pos);
				s_val = s_point.substr(pos, pos2++ - pos);
				point.y = std::strtof(s_val.c_str(), nullptr);
				//
				s_val = s_point.substr(++pos2);
				point.z = std::strtof(s_val.c_str(), nullptr);
				//
				path.push_back(point);
			}
		}
	}

	//
	return path;
}

StatsLoader::~StatsLoader()
{

}

Stats * StatsLoader::get_Stat(std::string temp)
{
	return statsList.find(temp)->second;
}

