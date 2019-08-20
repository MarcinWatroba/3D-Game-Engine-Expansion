#pragma once

class RenderQuad
{
private:
	unsigned int quadVAO;
	unsigned int hdrFBO;
	unsigned int colorBuffers[2];
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];
	bool horizontal;
public:

	RenderQuad();
	~RenderQuad();
	void pingPong(bool horizont, bool first);
	void render();
	unsigned int getFBO();
	//RenderQuad(const RenderQuad &p_NewComp_In);

	//Render
	//void render(int draw_Mode_In, int draw_Shape_In, Shader * p_Shader_In);

}; 