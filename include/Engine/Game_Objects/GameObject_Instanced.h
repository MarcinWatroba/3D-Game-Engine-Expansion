#pragma once

#include <Engine\Game_Objects\Game_Object.h>
#include <glm\glm.hpp>

struct Particle {
	glm::vec3 position, speed;
	glm::vec3 lastPos;
	unsigned char r, g, b; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if < 0 : dead and unused.
	float cameraDistance;

};

class GameObject_Instanced : public Game_Object
{
private:
	int VAO;
	int i_Buffer;
	int index_Size;
	unsigned int maxParticles;
	float particle_Life;
	glm::vec3 range, particle_Speed, colour;
	//positions of particles
	float particlePositions[300000];
	//container of particles
	Particle ParticlesContainer[100000];
	int ParticlesCount, iParticle, LastUsedParticle;
	float random, random2, random3;
	//std::vector<float> positions;
	glm::mat4 get_ParentMatrix();
public:
	//Constructor
	GameObject_Instanced();

	GameObject_Instanced(const GameObject_Instanced & p_NewObject_In);

	void update();
	//set max particle amount
	void setMax(unsigned int max_In);
	//set particle rendering time(life)
	void set_Life(float life_In);
	//set range of xyz where particles can spawn
	void set_Range(glm::vec3 range_In);
	//speed of particles
	void set_Particle_Speed(glm::vec3 speed_In);
	//colour of particles
	void set_Colour(glm::vec3 colour_In);
	//get maximum amount of particles
	unsigned int getMax();
	//get life of particles
	float get_Life();
	//update each particle that is being rendered
	void update_Particles(float t, float y, float z, glm::vec3 delta);
	glm::vec3 get_Range();
	glm::vec3 get_Particle_Speed();
	glm::vec3 get_Colour();
	// finds particle that can be rerendered
	int FindUnusedParticle();
	void force_Update();
	void add_Component(std::string s_Name_In, Component* p_Component_In);
	void add_Texture(std::string s_Name_In, Texture* p_Texture_In);
	void renderDepth(Shader * p_Shader_In);
	void render(Shader* p_Shader_In);
	void render(Shader * p_Shader_In, bool skybox);
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

	void set_Tiles(glm::vec2 v2_Tiles_In);
};