#include <Engine\Component\Transform_Instanced.h>
#include <Engine\Creators\Shader.h>
#include <glad\glad.h>
#include <vector>

Transform_Instanced::Transform_Instanced(const Transform_Instanced & p_NewComp_In) : Transform_3D(p_NewComp_In)
{}
std::string Transform_Instanced::get_Type()
{
	return "Transform_Instanced";
}