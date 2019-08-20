#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

class Shader;

class Camera
{
protected:	
	glm::mat4 mat4_Projection;
	glm::vec2 v2_LastMousePos; // Last pos of the mouse

public:
	//Constructor
	Camera() {};
	virtual void update() = 0;
	virtual void update_Shader(Shader* p_Shader_In) = 0;
};