#include <Engine\Component\Transform.h>

Transform::Transform()
{
	b_Update = false;
}

Transform::~Transform()
{}

Transform::Transform(const Transform & p_NewComp_In)
{
	mat4_Model = p_NewComp_In.mat4_Model;
	b_Update = p_NewComp_In.b_Update;
}

std::string Transform::get_Type()
{
	return "Transform";
}

glm::mat4* Transform::get_ModelMatrix()
{
	return &mat4_Model;
}

void Transform::force_Update()
{
	b_Update = true;
}