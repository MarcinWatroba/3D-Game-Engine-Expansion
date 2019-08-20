#pragma once
#include <Engine\Component\Transform.h>

class Transform_2D : public Transform
{
private:
	glm::vec2 v2_Position; // Position
	glm::vec2 v2_Size; // Scale
	float f_Angle; // Orientation
	glm::vec2 v2_Origin;  // Origin
	float f_Layer; // Layer
public:
	Transform_2D() {};
	~Transform_2D() {};
	Transform_2D(const Transform_2D &p_NewComp_In);
	std::string get_Type();
	//Update
	void update();
	void update(glm::mat4 mat4_ParentMatrix_In);
	void update_Shader(Shader* p_Shader_In);
	void set_Position(glm::vec2 v2_Position_In);
	void set_Origin(glm::vec2 v2_Origin_In);
	void set_Size(glm::vec2 v2_Size_In);
	void set_Layer(float f_Layer_In);
	void set_Angle(float f_Angle_In);
	glm::vec2 get_Size();
	glm::vec2 get_Position();
	glm::vec2 get_Origin();
	float get_Layer();
	float get_Angle();
	glm::mat4 get_ModelMatrix();
};