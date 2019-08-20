#include <Engine\Component\Transform_2D.h>
#include <Engine\Creators\Shader.h>
#include <glad\glad.h>

Transform_2D::Transform_2D(const Transform_2D & p_NewComp_In) : Transform(p_NewComp_In)
{
	v2_Position = p_NewComp_In.v2_Position;
	v2_Size = p_NewComp_In.v2_Size;
	f_Angle = p_NewComp_In.f_Angle;
	v2_Origin = p_NewComp_In.v2_Origin;
	f_Layer = p_NewComp_In.f_Layer;
}

void Transform_2D::update()
{
	if (b_Update)
	{
		mat4_Model = glm::mat4();
		mat4_Model = glm::translate(mat4_Model, glm::vec3(v2_Position.x + (v2_Size.x / 2.f), v2_Position.y + (v2_Size.y / 2.f), f_Layer));
		mat4_Model = glm::translate(mat4_Model, glm::vec3(-v2_Origin.x, -v2_Origin.y, 0.f));
		mat4_Model = glm::rotate(mat4_Model, glm::radians(f_Angle), glm::vec3(0.f, 0.f, 1.f));
		mat4_Model = glm::scale(mat4_Model, glm::vec3(v2_Size.x / 2.f, v2_Size.y / 2.f, 1.f)); 
		b_Update = false;
	}
}

void Transform_2D::update(glm::mat4 mat4_ParentMatrix_In)
{
	if (b_Update)
	{
		mat4_Model = mat4_ParentMatrix_In;
		mat4_Model = glm::translate(mat4_Model, glm::vec3(v2_Position.x + (v2_Size.x / 2.f), v2_Position.y + (v2_Size.y / 2.f), f_Layer));
		mat4_Model = glm::translate(mat4_Model, glm::vec3(-v2_Origin.x, -v2_Origin.y, 0.f)); 
		mat4_Model = glm::rotate(mat4_Model, glm::radians(f_Angle), glm::vec3(0.f, 0.f, 1.f));
		mat4_Model = glm::translate(mat4_Model, glm::vec3(v2_Origin.x, v2_Origin.y, 0.f));
		mat4_Model = glm::scale(mat4_Model, glm::vec3(v2_Size.x / 2.f, v2_Size.y / 2.f, 1.f));
		b_Update = false;
	}
}

void Transform_2D::update_Shader(Shader* p_Shader_In)
{
	//Uniform locations
	int modelLoc = glGetUniformLocation(p_Shader_In->get_Program(), "model");
	//Pass them to shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mat4_Model));
}

void Transform_2D::set_Position(glm::vec2 v2_Position_In)
{
	b_Update = true;
	v2_Position = glm::vec2(v2_Position_In.x, v2_Position_In.y);
}

void Transform_2D::set_Origin(glm::vec2 v2_Origin_In)
{
	b_Update = true;
	v2_Origin = glm::vec2(v2_Origin_In.x, v2_Origin_In.y);
}

void Transform_2D::set_Size(glm::vec2 v2_Size_In)
{
	b_Update = true;
	v2_Size = glm::vec3(v2_Size_In.x, v2_Size_In.y, 0.f);
}

void Transform_2D::set_Layer(float f_Layer_In)
{
	f_Layer = f_Layer_In;
}

void Transform_2D::set_Angle(float f_Angle_In)
{
	f_Angle = f_Angle_In;
}

glm::vec2 Transform_2D::get_Position()
{
	return v2_Position;
}

glm::vec2 Transform_2D::get_Origin()
{
	return v2_Origin;
}

glm::vec2 Transform_2D::get_Size()
{
	return v2_Size;
}

float Transform_2D::get_Layer()
{
	return f_Layer;
}

float Transform_2D::get_Angle()
{
	return f_Angle;
}

std::string Transform_2D::get_Type()
{
	return "Transform_2D";
}

glm::mat4 Transform_2D::get_ModelMatrix()
{
	return mat4_Model;
}
