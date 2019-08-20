#pragma once
#include <Engine\Component/Transform_3D.h>

class Transform_Instanced : public Transform_3D
{

private:
	//void toVector(float i_Array_In[]);
public:
	Transform_Instanced() {};
	~Transform_Instanced() {};
	Transform_Instanced(const Transform_Instanced & p_NewComp_In);
	std::string get_Type();
};