#pragma once

#include <glm\glm.hpp>
#include <vector>

class Stats
{
private:
	float health;
	float ammo;
	float mass;
	bool gravity;
	std::vector<glm::vec3> path;
public:
	Stats(float inhealth, float inammo, float inmass, bool ingravity);
	~Stats();
	Stats(const Stats &stat);
	float getHealth() { return health; }
	float getAmmo() { return ammo; }
	float getMass() { return mass; }
	bool getGravity() { return gravity; }
	std::vector<glm::vec3> getPath() { return path; }
	void addPathPoint(glm::vec3 point);
};