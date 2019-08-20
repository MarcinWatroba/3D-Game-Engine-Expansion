#include <Engine\Stats\Stats.h>

Stats::Stats(float inhealth,float inammo,float inmass, bool ingravity)
{
	health = inhealth;
	ammo = inammo;
	mass = inmass;
	gravity = ingravity;
}
Stats::~Stats()
{

}
Stats::Stats(const Stats &stat)
{
	health = stat.health;
	ammo = stat.ammo;
	mass = stat.mass;
	gravity = stat.gravity;
}

void Stats::addPathPoint(glm::vec3 point)
{
	path.push_back(point);
}