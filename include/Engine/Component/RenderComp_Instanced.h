#pragma once

#include <Engine\Component\RenderComp.h>
class Mesh_Instanced;

class RenderComp_Instanced : public RenderComp
{
private:

public:
	void render(int i_DrawMode_In, int i_DrawShape_In, Shader* p_Shader_In);
	//render particle with these parameters
	void renderInstanced(int draw_Mode_In, int draw_Shape_In, Shader * p_Shader_In, int max, int count, float positions[], glm::vec3 colour_In);

	RenderComp_Instanced();
	~RenderComp_Instanced();
	RenderComp_Instanced(const RenderComp_Instanced &p_NewComp_In);
	std::string get_Type();
};