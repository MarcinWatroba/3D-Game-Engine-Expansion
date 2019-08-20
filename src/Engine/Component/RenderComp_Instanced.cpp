#include <Engine\Component\RenderComp_Instanced.h>
#include <Engine\Mesh\Mesh_Instanced.h>
#include <Engine\Creators\Shader.h>
#include <glad\glad.h>

RenderComp_Instanced::RenderComp_Instanced() {};
RenderComp_Instanced::~RenderComp_Instanced() {};
RenderComp_Instanced::RenderComp_Instanced(const RenderComp_Instanced &p_NewComp_In) : RenderComp(p_NewComp_In) {}

std::string RenderComp_Instanced::get_Type()
{
	return "RenderComp_Instanced";
}

void RenderComp_Instanced::renderInstanced(int draw_Mode_In, int draw_Shape_In, Shader* p_Shader_In, int max, int count, float positions[], glm::vec3 colour_In)
{
	//Bind lightning maps
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, get_Texture("Diffuse_Map"));
	GLint colourLoc = glGetUniformLocation(p_Shader_In->get_Program(), "colour");
	glUniform3f(colourLoc, colour_In.x, colour_In.y, colour_In.z);


	glBindBuffer(GL_ARRAY_BUFFER, p_Mesh->get_InstanceBufferHandle());
	glBufferData(GL_ARRAY_BUFFER, max * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, to improve streaming performance.
	glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(GLfloat) * 3, positions);


	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 1);

	////Draw
	glBindVertexArray(p_Mesh->get_VAO());
	glDrawElementsInstanced(GL_TRIANGLES, p_Mesh->get_SizeOfIndices(), GL_UNSIGNED_INT, 0, count);   

	glBindVertexArray(0);
}

void RenderComp_Instanced::render(int i_DrawMode_In, int i_DrawShape_In, Shader * p_Shader_In)
{

}