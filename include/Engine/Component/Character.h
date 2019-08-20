#ifndef CHARACTER_H
#define CHARACTER_H

#include <Engine\Component\Component.h>
#include <Engine\Stats\Stats.h>
#include <vector>

class Character : public Component
{
private:
	float health;
	float numberOfBullets;
	std::vector<glm::vec3> path;
	std::string typeOfChar;
	bool endLevel = false;
public:
	Character(std::string inType = "");
	~Character() {}
	Character(const Character &obj);
	Character(Stats* stat);
	std::string get_Type();
	void setHealth(float temp);
	void setNumberOfBullets(float newAmmo);
	void setEndLevel(bool temp);

	void loseLife();
	void gainLife(float temp);
	void loseBullets();
	void gainBullets(float temp);

	float getHealth() { return health; }
	float getNumberOfBullets() { return numberOfBullets; }
	bool getEndLevel() { return endLevel; }

	std::vector<glm::vec3>* getPath();
};
#endif