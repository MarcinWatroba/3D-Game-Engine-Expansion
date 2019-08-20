#pragma once
#include <string>
#include <glm\glm.hpp>

class Texture
{
private:
	unsigned int ui_Texture; // Texture

	int i_Width;
	int i_Height;

	//Size
	glm::vec2 v2_Size;
	unsigned char* puc_Image; // Our texture
	std::string s_ID;
public:
	//Constructor
	Texture() {}

	//Main constructor
	Texture(const char* pc_TextureName_In, int i_TextureMode_In, int i_WrappingS_In, 
		int i_WrappingT_In, int i_FilterMin_In, int i_FilterMag_In, int i_ColourChannel_In, int i_ColourFormat_In, bool b_GenMipmap_In, std::string s_ID_In);

	Texture(std::string(&pc_TextureNames_In)[6], int i_FilterMin_In, int i_FilterMag_In, int i_ColourChannel_In, int i_ColourFormat_In);

	//Get loaded texture
	unsigned int get_Texture();
	std::string get_ID();
	glm::vec2 get_Size();
};
