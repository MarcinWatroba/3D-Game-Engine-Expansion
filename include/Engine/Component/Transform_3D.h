#pragma once
#include <Engine\Component\Transform.h>

class Transform_3D : public Transform
{
protected:
	glm::vec3 v3_Position; // Position
	glm::vec3 v3_Scale; // Scale
	glm::quat quat_Orientation; // Orientation
	glm::vec3 v3_Origin;  // Origin
public:
	Transform_3D() {};
	~Transform_3D() {};
	Transform_3D(const Transform_3D &p_NewComp_In);
	std::string get_Type();
	//------------------Transformations-------------------------
	void set_Position(glm::vec3 v3_Position_In); //Set position
	void set_Origin(glm::vec3 v3_Origin_In); //Set origin

	//Get origin
	glm::vec3 get_Origin();
	glm::vec3 get_Position();

	//Get direction vectors
	glm::vec3 get_Forward();
	glm::vec3 get_Right();
	glm::vec3 get_Up();

	//set Scale/Size
	void set_Scale(glm::vec3 v3_Scale_In);

	//Get Scale/Size
	glm::vec3& get_Scale();

	//Set rotation
	void set_Rotation(glm::quat quat_Rot_In);
	void set_Rotation(float f_Angle_In, glm::vec3 v3_RotAxis_In);

	//Updating roration
	void update_Rotation(glm::quat quat_Rot_In);
	glm::quat get_Rotation();

	void update(); // Update
	void update(glm::mat4 parentMatrix_In); // Update child
	void update_Shader(Shader* p_Shader_In);

	glm::mat4 get_ModelMatrix();
};