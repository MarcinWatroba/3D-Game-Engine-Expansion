#include <Engine\Lighting\Point_Light.h>
#include <glad\glad.h>
#include <Engine\Creators\Shader.h>


Point_Light::Point_Light(glm::vec3 v3_Ambient_In, glm::vec3 v3_Diffuse_In, glm::vec3 v3_Specular_In, float f_Radius_In) : Light(v3_Ambient_In, v3_Diffuse_In, v3_Specular_In)
{
	f_radius = f_Radius_In;
}
Point_Light::Point_Light(const Point_Light & point_Light) : Light(point_Light)
{
	f_radius = point_Light.f_radius;
}

std::string Point_Light::get_Type()
{
	return "Point_Light";
}

void Point_Light::set_Radius(float f_Radius_In)
{
	f_radius = f_Radius_In;
}

float Point_Light::get_Radius()
{
	return f_radius;
}

void Point_Light::set_ID(int i_ID_In)
{
	i_ID = i_ID_In;
}

int Point_Light::get_ID()
{
	return i_ID;
}

glm::vec3 Point_Light::get_Color()
{
	return v3_Diffuse;
}

void Point_Light::update_Shader(Shader* p_Shader_In)
{
	std::string point_Light = "point_Light[" + std::to_string(i_ID) + "]";

	std::string pos = point_Light + ".position";
	GLint posLoc = glGetUniformLocation(p_Shader_In->get_Program(), pos.c_str());
	glUniform3f(posLoc, get_Position().x, get_Position().y, get_Position().z);

	std::string amb = point_Light + ".ambient";
	GLint ambientLoc = glGetUniformLocation(p_Shader_In->get_Program(), amb.c_str());
	glUniform3f(ambientLoc, v3_Ambient.x, v3_Ambient.y, v3_Ambient.z);

	std::string diff = point_Light + ".diffuse";
	GLint diffuseLoc = glGetUniformLocation(p_Shader_In->get_Program(), diff.c_str());
	glUniform3f(diffuseLoc, v3_Diffuse.x, v3_Diffuse.y, v3_Diffuse.z);

	std::string spec = point_Light + ".specular";
	GLint specularLoc = glGetUniformLocation(p_Shader_In->get_Program(), spec.c_str());
	glUniform3f(specularLoc, v3_Specular.x, v3_Specular.y, v3_Specular.z);

	std::string rad = point_Light + ".radius";
	GLint radiusLoc = glGetUniformLocation(p_Shader_In->get_Program(), rad.c_str());
	glUniform1f(radiusLoc, f_radius);

}

void Point_Light::set_Bulb_Color(Shader* p_Shader_In)
{
	//std::string rad = point_Light + ".radius";

	GLint colorLoc = glGetUniformLocation(p_Shader_In->get_Program(), "light_Colour");
	glUniform3f(colorLoc, v3_Diffuse.x, v3_Diffuse.y, v3_Diffuse.z);
}

