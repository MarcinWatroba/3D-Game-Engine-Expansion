#pragma once
#include <Engine\Game_Objects\Camera.h>

class Camera_3D : public Camera
{
protected:
	glm::mat4 mat4_View;
	glm::mat4 mat4_Shadow_View;
	glm::vec2 v2_LastClickPos;
	//Deltas for camera movement
	float f_DeltaX;
	float f_DeltaY;
	float f_DeltaZ;

	float f_Speed; // Camera speed

	//Position
	float f_Yaw;
	float f_Pitch;
	float f_YawOther;
	float f_PitchOther;
	float f_YawDelta;
	float f_PitchDelta;

	//Vectors
	glm::vec3 vec3_EyePos; // Camera position
	glm::quat quat_Orientation; // Camera orientation

	//Matrices
	glm::mat4 mat4_Roll;
	glm::mat4 mat4_Pitch;
	glm::mat4 mat4_Yaw;



	//Matrices
public:
	Camera_3D() {};
	Camera_3D(float f_FoV_In, float f_Ratio_In, float f_NearPlane_In, float f_FarPlane_In);

	void update();
	void update_Shader(Shader* p_Shader_In);
	//Set camera position
	void set_CameraPos(glm::vec3 v3_Pos_In);

	//Get camera position
	glm::vec3 get_CameraPos();

	float get_YawDelta() { return f_YawDelta; }
	float get_PitchDelta() { return f_PitchDelta; }

	glm::quat get_Quat();

	//Move camera using keyboard
	void move_Keyboard(float f_Delta_In);

	int get_CameraSide();

	//Move camera using mouse
	void move_Mouse(float f_Delta_In, glm::vec2 v2_MousePos_In, glm::vec2 v2_WindowSize_In);

	//--------Keyboard movement-------------------
	void move_Forward();
	void move_Backward();
	void move_Right();
	void move_Left();
	void fly_Up();
	void fly_Down();
	void reset();
	void move_MouseEditor(float f_Delta_In, glm::vec2 v2_MousePos_In, glm::vec2 v2_WindowSize_In);
	glm::vec2 get_PitchYaw(float f_Delta_In, glm::vec2 v2_MousePos_In, glm::vec2 v2_WindowSize_In);

	void set_Speed(float f_Speed_In);
};