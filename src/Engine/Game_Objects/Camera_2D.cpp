#include <Engine\Game_Objects\Camera_2D.h>
#include <Engine/Creators/Shader.h>

#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


Camera_2D::Camera_2D(float f_Left_In, float f_Right_In, float f_Bottom_In, float f_Top_In)
{
	mat4_Projection = glm::ortho(f_Left_In, f_Right_In, f_Bottom_In, f_Top_In);
}

void Camera_2D::update()
{

}

void Camera_2D::update_Shader(Shader* p_Shader_In)
{
	//Send them to shader
	GLint projLoc = glGetUniformLocation(p_Shader_In->get_Program(), "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mat4_Projection));
}
