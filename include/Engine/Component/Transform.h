#pragma once

#include <Engine/Component/Component.h>

class Shader;

class Transform : public Component
{
protected:
	//const int MaxParticles = 10000;
	glm::mat4 mat4_Model; // Model matrix
	bool b_Update; // Update
public:
	//Constructor
	Transform();
	~Transform();
	Transform(const Transform &p_NewComp_In);
	std::string get_Type();
	//Updates
	virtual void update() = 0;
	virtual void update(glm::mat4 mat4_ParentMatrix_In) = 0; // Updates for attached child
	virtual void update_Shader(Shader* p_Shader_In) = 0; // Update shader
	
	//Get model matrix
	glm::mat4* get_ModelMatrix();

	//Force updates on children
	void force_Update();
};