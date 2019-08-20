#pragma once
#include <Engine\Game_Objects\Game_Object.h>


class GameObject_2D : public Game_Object
{
public:
	GameObject_2D() {};
	GameObject_2D(const GameObject_2D &p_NewObject_In);
	glm::mat4 get_ParentMatrix();
	void add_Component(std::string s_Name_In, Component* p_Component_In);
	void add_Texture(std::string s_Name_In, Texture* p_Texture_In);
	void set_Tiles(glm::vec2 v2_Tiles_In);
	void update();
	void force_Update();
	void render(Shader* p_Shader_In);
	void render(Shader * p_Shader_In, bool skybox);
	void clean_Up();
	void renderDepth(Shader* p_Shader_In);
	void set_Position(glm::vec2 v2_Position_In);
	void set_Origin(glm::vec2 v2_Origin_In);
	void set_Size(glm::vec2 v2_Size_In);
	void set_Layer(float f_Layer_In);
	void set_Angle(float f_Angle_In);
	void set_Colour(glm::vec3 rgb_Colour_In);

	glm::vec2 get_Position();
	glm::vec2 get_Origin();
	glm::vec2 get_Size();
	float get_Layer();
	float get_Angle();

	std::string get_Type();
};