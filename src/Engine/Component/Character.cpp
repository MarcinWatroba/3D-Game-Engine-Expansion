#include <Engine\Component\Character.h>
Character::Character(std::string inType)
{

}

Character::Character(const Character &obj)
{
	health = obj.health;
	numberOfBullets = obj.numberOfBullets;
	path = std::vector<glm::vec3>(obj.path);
}
Character::Character(Stats * stat)
{
	health = stat->getHealth();
	numberOfBullets = stat->getAmmo();
	path = std::vector<glm::vec3>(stat->getPath());
}

std::vector<glm::vec3>* Character::getPath()
{
	return &path;
}

std::string Character::get_Type()
{
	return "Character";
}

void Character::setHealth(float temp)
{
	health = temp;
}

void Character::setNumberOfBullets(float temp)
{
	numberOfBullets = temp;
}

void Character::setEndLevel(bool temp)
{
	endLevel = temp;
}

void Character::loseLife()
{
	health--;
}

void Character::gainLife(float temp)
{
	health += temp;
}

void Character::loseBullets()
{
	numberOfBullets--;
}

void Character::gainBullets(float temp)
{
	numberOfBullets += temp;
}