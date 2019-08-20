#include <Engine\Game_Objects\Camera_3D.h>
#include <Engine/Creators/Shader.h>

#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

Camera_3D::Camera_3D(float f_FoV_In, float f_Ratio_In, float f_NearPlane_In, float f_FarPlane_In)
{
	vec3_EyePos = glm::vec3(0.f, 0.f, 0.f);
	mat4_Projection = glm::perspective(glm::radians(f_FoV_In), f_Ratio_In, f_NearPlane_In, f_FarPlane_In);
	mat4_View = glm::mat4(1.f);
	f_Pitch = 0.f;
	f_Yaw = 0.f;
	f_DeltaX = 0.f;
	f_DeltaY = 0.f;
	f_DeltaZ = 0.f;
}

void Camera_3D::update()
{
	//Create quaternions
	glm::quat quat_Pitch;
	glm::quat quat_Yaw;

	quat_Pitch = glm::angleAxis(glm::radians(f_Pitch), glm::vec3(1.f, 0.f, 0.f)); //Pitch is a rotation around X axis // Usually called the right vector
	quat_Yaw = glm::angleAxis(glm::radians(f_Yaw), glm::vec3(0.f, 1.f, 0.f)); // Yaw is a rotation around Y axis // Usually called the upper vector

	//Apply multiplication
	//Pitch and yaw must be applied in that order to avoid tilting
	glm::quat quat_Temp_01 = quat_Pitch * quat_Orientation;
	glm::quat quat_Temp_02 = quat_Temp_01 * quat_Yaw;
	quat_Orientation = quat_Temp_02; // Apply rotations

	//Normalization is needed, because quaterion de-normalizes every frame
	quat_Orientation = glm::normalize(quat_Orientation);

	//Convert quat to matrix
	glm::mat4 mat4_CombinedRot = glm::mat4_cast(quat_Orientation);

	//Translate now
	glm::mat4 mat4_TransView = glm::mat4(1.f);
	mat4_TransView = glm::translate(mat4_TransView, vec3_EyePos);
	mat4_View = mat4_CombinedRot * mat4_TransView; //Multiply them all

	mat4_Shadow_View = glm::mat4(1.0f);
	mat4_Shadow_View[3][0] = mat4_View[3][0];
	mat4_Shadow_View[3][1] = mat4_View[3][1];
	mat4_Shadow_View[3][2] = mat4_View[3][2];

	f_Pitch = f_Yaw = 0;
}

void Camera_3D::update_Shader(Shader* p_Shader_In)
{
	//Send them to shader
	GLint viewLoc = glGetUniformLocation(p_Shader_In->get_Program(), "view");
	GLint projLoc = glGetUniformLocation(p_Shader_In->get_Program(), "projection");
	GLint shadowViewLoc = glGetUniformLocation(p_Shader_In->get_Program(), "shadowViewMat");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mat4_View));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mat4_Projection));
	glUniformMatrix4fv(shadowViewLoc, 1, GL_FALSE, glm::value_ptr(mat4_Shadow_View));
}

void Camera_3D::set_Speed(float f_Speed_In)
{
	f_Speed = f_Speed_In;
}

int Camera_3D::get_CameraSide()
{
	glm::vec3 euler = glm::eulerAngles(quat_Orientation);

	if (euler.y >= 0) return 1;
	else return -1;
}

void Camera_3D::move_Keyboard(float f_Delta_In)
{
	//Extract view matrix to calculate movement
	glm::vec3 forward(mat4_View[0][2], mat4_View[1][2], mat4_View[2][2]);
	glm::vec3 strafe(mat4_View[0][0], mat4_View[1][0], mat4_View[2][0]);

	//Keyboard movement
	vec3_EyePos += (-f_DeltaZ * forward + f_DeltaX * strafe + f_DeltaY * glm::vec3(0.f, 1.f, 0.f));
}

void Camera_3D::move_Mouse(float f_Delta_In, glm::vec2 v2_MousePos_In, glm::vec2 v2_WindowSize_In)
{
	//Mouse movement
	v2_LastClickPos.x += v2_MousePos_In.x - (v2_WindowSize_In.x / 2.f);
	v2_LastClickPos.y += v2_MousePos_In.y - (v2_WindowSize_In.y / 2.f);

	//Mouse movement
	float deltaMouse_X = v2_LastClickPos.x - v2_LastMousePos.x;
	float deltaMouse_Y = v2_LastClickPos.y - v2_LastMousePos.y;

	//Sensitivity
	float mouseX_Sensitivity = 1.f * f_Delta_In;
	float mouseY_Sensitivity = 1.f * f_Delta_In;

	//Calculate yaw and pitch
	f_YawDelta = mouseX_Sensitivity * deltaMouse_X;
	f_Yaw += f_YawDelta;
	f_PitchDelta = mouseY_Sensitivity * deltaMouse_Y;
	f_Pitch += f_PitchDelta;

	v2_LastMousePos = v2_LastClickPos;
}

void Camera_3D::move_Forward()
{
	f_DeltaZ = -f_Speed;
}

void Camera_3D::move_Backward()
{
	f_DeltaZ = f_Speed;
}

void Camera_3D::move_Left()
{
	f_DeltaX = f_Speed;
}

void Camera_3D::move_Right()
{
	f_DeltaX = -f_Speed;
}

void Camera_3D::fly_Up()
{
	f_DeltaY = -f_Speed;
}

void Camera_3D::fly_Down()
{
	f_DeltaY = f_Speed;
}

void Camera_3D::reset()
{
	f_DeltaZ = 0.f;
	f_DeltaY = 0.f;
	f_DeltaX = 0.f;
	f_YawOther = f_PitchOther = 0.f;
}

void Camera_3D::move_MouseEditor(float f_Delta_In, glm::vec2 v2_MousePos_In, glm::vec2 v2_WindowSize_In)
{
	//Mouse movement
	v2_LastClickPos.x += v2_MousePos_In.x - (v2_WindowSize_In.x / 2.f);
	v2_LastClickPos.y += v2_MousePos_In.y - (v2_WindowSize_In.y / 2.f);

	//Mouse movement
	float deltaMouse_X = v2_LastClickPos.x - v2_LastMousePos.x;
	float deltaMouse_Y = v2_LastClickPos.y - v2_LastMousePos.y;

	//Sensitivity
	float mouseX_Sensitivity = 1.f * f_Delta_In;
	float mouseY_Sensitivity = 1.f * f_Delta_In;

	//Calculate yaw and pitch
	f_YawOther += mouseX_Sensitivity * deltaMouse_X;
	f_PitchOther += mouseY_Sensitivity * deltaMouse_Y;

	v2_LastMousePos = v2_LastClickPos;

	//Extract view matrix to calculate movement
	glm::vec3 forward(mat4_View[0][2], mat4_View[1][2], mat4_View[2][2]);
	glm::vec3 strafe(mat4_View[0][0], mat4_View[1][0], mat4_View[2][0]);

	f_DeltaX = -f_YawOther;
	f_DeltaY = f_PitchOther;

	//Keyboard movement
	vec3_EyePos += (-f_DeltaZ * forward + f_DeltaX * strafe + f_DeltaY * glm::vec3(0.f, 1.f, 0.f));
}

glm::vec2 Camera_3D::get_PitchYaw(float f_Delta_In, glm::vec2 v2_MousePos_In, glm::vec2 v2_WindowSize_In)
{
	//Mouse movement
	v2_LastClickPos.x += v2_MousePos_In.x - (v2_WindowSize_In.x / 2.f);
	v2_LastClickPos.y += v2_MousePos_In.y - (v2_WindowSize_In.y / 2.f);

	//Mouse movement
	float deltaMouse_X = v2_LastClickPos.x - v2_LastMousePos.x;
	float deltaMouse_Y = v2_LastClickPos.y - v2_LastMousePos.y;

	//Sensitivity
	float mouseX_Sensitivity = 1.f * f_Delta_In;
	float mouseY_Sensitivity = 1.f * f_Delta_In;

	//Calculate yaw and pitch
	f_YawOther += mouseX_Sensitivity * deltaMouse_X;
	f_PitchOther += mouseY_Sensitivity * deltaMouse_Y;

	v2_LastMousePos = v2_LastClickPos;

	return glm::vec2(f_YawOther, f_PitchOther);
}

void Camera_3D::set_CameraPos(glm::vec3 v3_Pos_In)
{
	vec3_EyePos = v3_Pos_In;
}

glm::vec3 Camera_3D::get_CameraPos()
{
	return -vec3_EyePos;
}

glm::quat Camera_3D::get_Quat()
{
	return quat_Orientation;
}