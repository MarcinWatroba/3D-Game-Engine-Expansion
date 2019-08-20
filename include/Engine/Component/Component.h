#pragma once

#include <memory>
#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


class Component
{
public:
	//Constructor
	Component();
	~Component();
	Component(const Component &p_NewComp_In);
	//get Type
	virtual std::string get_Type() = 0;
};