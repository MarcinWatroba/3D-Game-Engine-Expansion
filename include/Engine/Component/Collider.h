#ifndef COLLIDER_H
#define COLLIDER_H

#include <glad\glad.h>
#include <iostream>
#include <map>
#include <Engine/Component/Component.h>

class Collider: public Component
{
private:
public:
	Collider();
	~Collider();
	Collider(const Collider &p_NewComp_In);
	std::string get_Type() = 0;
};

#endif;