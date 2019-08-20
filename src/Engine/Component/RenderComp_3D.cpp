#include <Engine\Component\RenderComp_3D.h>
#include <Engine\Mesh\Mesh_3D.h>
#include <Engine\Creators\Shader.h>
#include <glad\glad.h>
#include <Engine\Creators\Texture.h>

RenderComp_3D::RenderComp_3D() {};
RenderComp_3D::~RenderComp_3D() {};
RenderComp_3D::RenderComp_3D(const RenderComp_3D & p_NewComp_In) : RenderComp(p_NewComp_In)
{
	f_Shininess = p_NewComp_In.f_Shininess;
}
std::string RenderComp_3D::get_Type()
{
	return "RenderComp_3D";
}

void RenderComp_3D::renderDepth(int draw_Mode_In, int draw_Shape_In, Shader* p_Shader_In)
{
	glBindVertexArray(p_Mesh->get_VAO());
	glDrawElements(GL_TRIANGLES, p_Mesh->get_SizeOfIndices(), GL_UNSIGNED_INT, 0);
}

void RenderComp_3D::renderInstanced(int draw_Mode_In, int draw_Shape_In, Shader * p_Shader_In, int max, int count, float positions[], glm::vec3 colour_In)
{
}

void RenderComp_3D::render(int draw_Mode_In, int draw_Shape_In, Shader* p_Shader_In)
{
	//Bind lightning maps
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, get_Texture("Diffuse_Map")->get_Texture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, get_Texture("Specular_Map")->get_Texture());

	//Pass texture to the shader
	GLint diffuseLoc = glGetUniformLocation(p_Shader_In->get_Program(), "material.diffuse");
	GLint specularLoc = glGetUniformLocation(p_Shader_In->get_Program(), "material.specular");

	glUniform1i(diffuseLoc, 0);
	glUniform1i(specularLoc, 1);

	//Uniform locations
	GLint tileLoc = glGetUniformLocation(p_Shader_In->get_Program(), "tiling");
	GLint matShiniessLoc = glGetUniformLocation(p_Shader_In->get_Program(), "material.shininess");

	//Pass them to shaders
	glUniform2f(tileLoc, get_Tiles().x, get_Tiles().y);
	glUniform1f(matShiniessLoc, get_Shininess());
	//Draw
	glBindVertexArray(p_Mesh->get_VAO());
	glDrawElements(GL_TRIANGLES, p_Mesh->get_SizeOfIndices(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RenderComp_3D::render(int draw_Mode_In, int draw_Shape_In, Shader* p_Shader_In, bool skybox)
{
	GLint textureLoc = glGetUniformLocation(p_Shader_In->get_Program(), "cubemap");
	glUniform1i(textureLoc, 0);

	glActiveTexture(GL_TEXTURE0);
	int textureID = get_Texture("Diffuse_Map")->get_Texture();
	glBindTexture(GL_TEXTURE_CUBE_MAP, get_Texture("Diffuse_Map")->get_Texture());
	glDepthMask(GL_FALSE);
	glBindVertexArray(p_Mesh->get_VAO());
	glDrawElements(GL_TRIANGLES, p_Mesh->get_SizeOfIndices(), GL_UNSIGNED_INT, 0);	//draw elements as triangles 
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

void RenderComp_3D::set_Shininess(float f_Shininess_In)
{
	f_Shininess = f_Shininess_In;
}

float RenderComp_3D::get_Shininess()
{
	return f_Shininess;
}
