#pragma once

//Obligatory
///Engine classes///
#include <Engine\Creators\Shader.h>
#include <Engine\Creators\Texture.h>
#include <Engine\Mesh\Mesh_3D.h>
#include <Engine\Mesh\Mesh_2D.h>
#include <Engine\Mesh\Mesh_Instanced.h>
//sound
#include <Engine\Audio\AudioEngine.h>
#include <Engine\Audio\Sound.h>
///3rd party///
#include <TinyXML2\tinyxml2.h>
#include <Engine\GUI\Font.h>

class Loader
{
private:
	//Models
	std::map<std::string, Mesh_3D*> mipo_Meshes3D;
	std::map<std::string, Mesh_Instanced*> mipo_MeshesInstanced;
	std::map<std::string, Mesh*> mipo_Meshes;
	//Shader
	std::map<std::string, Shader*> mipo_Shaders;
	//Texture files
	std::map<std::string, Texture*> mipo_TextureFiles;
	//Fonts
	std::map<std::string, Font*> mipo_Fonts;
	// Audio
	std::map<std::string, Sound*> snd_Audio;

	void ParseXML_Resources(const char* pc_FileName); // Parse the shaders
	glm::vec3 to3DVector(const char * pc_Vector3D_In);
	glm::vec2 to2DVector(const char * pc_Vector2D_In);
public:
	//Constructor
	Loader() {};

	//Start the maker
	void start();

	//Get the shader
	Shader* get_Shader(std::string s_Name_In);
	
	//Get texture
	Texture* get_Texture(std::string s_Name_In);

	//Get the model
	//Model* get_Model(int i_Model_ID);
	Mesh* get_Mesh(std::string s_Name_In);

	//Get font
	Font* get_Font(std::string s_Name_In);

	// Get the Audio
	Sound* get_Sound(std::string s_Name_In);

	std::map<std::string, Sound*> getAudioMap() { return snd_Audio; }

	//Clean up the memory
	void clean_Up();

	//std::map<int, Shader>* get_ShaderMap();
};