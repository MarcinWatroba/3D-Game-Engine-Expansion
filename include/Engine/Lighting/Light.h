#pragma once

#include <Engine/Game_Objects/GameObject_3D.h>
#include <Engine/Game_Objects/GameObject_Instanced.h>

class Light : public GameObject_3D
{
protected:
	//Ambient intensity
	glm::vec3 v3_Ambient;

	//Diffuse intensity
	glm::vec3 v3_Diffuse;

	//Speculat intensity
	glm::vec3 v3_Specular;

	//Assigned depth cube texture ID
	glm::uvec2 ui_Depth_Texture;
public:
	Light() {};

	//Main constructor
	Light(glm::vec3 v3_Ambient_In, glm::vec3 v3_Diffuse_In, glm::vec3 v3_Specular_In);

	Light(const Light &light);

	//Set ambient intensity
	void set_Ambient(glm::vec3 v3_Ambient_In);

	//Set diffuse intensity
	void set_Diffuse(glm::vec3 v3_Diffuse_In);

	//Set specular intensity
	void set_Specular(glm::vec3 v3_Specular_In);

	virtual void set_Depth_Texture(glm::uvec2 ui_Depth_In);

	virtual glm::uvec2 get_Depth_Texture();

	virtual void set_Radius(float f_Radius_In) = 0;

	virtual float get_Radius() = 0;

	//Update light

	virtual void update_Shader(Shader* p_Shader_In) = 0;

	virtual void set_Bulb_Color(Shader* p_Shader_In) = 0;

	virtual std::string get_Type() = 0;

	glm::vec3 get_Ambient();
	glm::vec3 get_Diffuse();
	glm::vec3 get_Specular();
};