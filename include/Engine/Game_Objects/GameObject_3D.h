#pragma once

#include <Engine\Game_Objects\Game_Object.h>
#include <glm\glm.hpp>
#include <vector>

class Bullet;
class Sound;

class GameObject_3D : public Game_Object
{
protected:
	glm::mat4 get_ParentMatrix();
	//Last Direction for Movement
	bool lastDir = false;
	bool firing = false;
	bool bulletActive = false;
	
	std::vector<GameObject_3D*> bulletList;
	float count = 0;
	float fireRate = 10;
	unsigned int bulletNumber = 0;
public:
	//Constructor
	GameObject_3D();
	GameObject_3D(const GameObject_3D &p_NewObject_In);

	void update();
	void force_Update();
	void add_Component(std::string s_Name_In, Component* p_Component_In);
	void add_Texture(std::string s_Name_In, Texture* p_Texture_In);
	void renderDepth(Shader * p_Shader_In);
	void render(Shader * p_Shader_In);
	void render(Shader * p_Shader_In, bool skybox);
	//void render(Shader * p_Shader_In, bool skybox);
	//void render(Shader* p_Shader_In);
	void clean_Up();

	//-----Set Transformations---------------------------
	void set_Position(glm::vec3 v3_Position_In);
	void set_Scale(glm::vec3 v3_Scale_In);
	void set_Origin(glm::vec3 v3_Origin_In);
	void set_Rotation(glm::quat quat_In);
	void update_Rotation(glm::quat quat_In);

	//-----Get Transformations---------------------------
	virtual glm::vec3 get_Position();
	virtual glm::vec3 get_Origin();
	virtual glm::quat get_Rotation();
	virtual glm::vec3 get_Scale();

	std::vector<GameObject_3D*> get_BulletList();

	//Only appropiate for 3D objects
	void set_Shininess(float f_Shiny_In);
	void set_Tiles(glm::vec2 v2_Tiles_In);

	void move(glm::vec3 v3_Direction_In, float f_Speed_In);
	void jump(glm::vec3 v3_Direction_In);
	void turn(float f_Angle_In, glm::vec3 v3_TurnAxis_In);
	void setFiring(bool input);
	void createBullet(Bullet* bulletTemplate, Sound*temp_Audio);
	void shootBullet();
	void resetCount();
	std::string get_Type();
};