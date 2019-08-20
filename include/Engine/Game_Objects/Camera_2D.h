#pragma once
#include <Engine\Game_Objects\Camera.h>

class Camera_2D : public Camera
{
public:
	Camera_2D() {};
	Camera_2D(float f_Left_In, float f_Right_In, float f_Bottom_In, float f_Top_In);

	void update();
	void update_Shader(Shader* p_Shader_In);
};


