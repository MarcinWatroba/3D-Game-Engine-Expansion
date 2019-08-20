#pragma once

#include <Engine\Component\RenderComp.h>

class RenderComp_2D : public RenderComp
{
public:
	//Render
	RenderComp_2D();
	~RenderComp_2D();
	RenderComp_2D(const RenderComp_2D &p_NewComp_In);
	std::string get_Type();

	void renderInstanced(int draw_Mode_In, int draw_Shape_In, Shader * p_Shader_In, int max, int count, float positions[], glm::vec3 colour_In);

	void render(int i_DrawMode_In, int i_DrawShape_In, Shader* p_Shader_In);
};