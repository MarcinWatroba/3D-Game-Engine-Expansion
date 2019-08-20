#pragma once

#include <Engine\Lighting\Light.h>

class Point_Light : public Light
{
private:

	float f_radius;
	//Attenutation constant


	int i_ID; // ID of the point light

public:
	//Constructor
	Point_Light() {};
	Point_Light(const Point_Light &point_Light);
	std::string get_Type();

	//Main constructor
	Point_Light(glm::vec3 v3_Ambient_In, glm::vec3 v3_Diffuse_In, glm::vec3 v3_Specular_In, float f_Radius_In);
	void set_ID(int i_ID_In);

	void set_Radius(float f_Radius_In);
	float get_Radius();

	//Update light
	void update_Shader(Shader* p_Shader_In);

	void set_Bulb_Color(Shader * p_Shader_In);

	int get_ID();

	glm::vec3 get_Color();

	float get_Constant();
	float get_Linear();
	float get_Quadratic();
};