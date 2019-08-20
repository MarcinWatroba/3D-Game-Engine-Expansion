#include <Engine\Creators\Texture.h>
#include <glad\glad.h>
#include <SOIL\SOIL.h>

Texture::Texture(const char* pc_TextureName_In, int i_TextureMode_In, int i_WrappingS_In,
	int i_WrappingT_In, int i_FilterMin_In, int i_FilterMag_In, int i_ColourChannel_In, int i_ColourFormat_In, bool b_GenMipmap_In, std::string s_ID_In)
{
	s_ID = s_ID_In;

	//Generate texture
	glGenTextures(1, &ui_Texture);
	glBindTexture(i_TextureMode_In, ui_Texture); // Bind it

										  //Texture wrapping
	glTexParameteri(i_TextureMode_In, GL_TEXTURE_WRAP_S, i_WrappingS_In);
	glTexParameteri(i_TextureMode_In, GL_TEXTURE_WRAP_T, i_WrappingT_In);

	//Texture filtering
	glTexParameteri(i_TextureMode_In, GL_TEXTURE_MIN_FILTER, i_FilterMin_In);
	glTexParameteri(i_TextureMode_In, GL_TEXTURE_MAG_FILTER, i_FilterMag_In);

	//Load the image
	puc_Image = SOIL_load_image(pc_TextureName_In, &i_Width, &i_Height, 0, i_ColourChannel_In);

	v2_Size = glm::vec2(i_Width, i_Height);

	//Decide what kind of texture it is
	switch (i_TextureMode_In)
	{
		//1D texture is not supported
	case GL_TEXTURE_1D:
		throw;
		break;

		//2D texture is supported
	case GL_TEXTURE_2D:



		glTexImage2D(i_TextureMode_In, 0, i_ColourFormat_In, i_Width, i_Height, 0, i_ColourFormat_In, GL_UNSIGNED_BYTE, puc_Image);
		break;

		//3D texture is not supported
	case GL_TEXTURE_3D:
		throw;
		break;
	}

	//Generate mipmap
	if (b_GenMipmap_In) glGenerateMipmap(i_TextureMode_In);

	//Free image
	SOIL_free_image_data(puc_Image);

	glBindTexture(i_TextureMode_In, 0);


}

Texture::Texture(std::string(&pc_TextureNames_In)[6], int i_FilterMin_In, int i_FilterMag_In, int i_ColourChannel_In, int i_ColourFormat_In)
{
	//Generate texture
	glGenTextures(1, &ui_Texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ui_Texture); // Bind it

													//Texture wrapping
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Texture filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load the image and add texture
	for (int i = 0; i < 6; i++)
	{
		puc_Image = SOIL_load_image(pc_TextureNames_In[i].c_str(), &i_Width, &i_Height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, i_Width, i_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, puc_Image);
	}


	//Free image
	SOIL_free_image_data(puc_Image);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

unsigned int Texture::get_Texture()
{
	return ui_Texture;
}

std::string Texture::get_ID()
{
	return s_ID;
}

glm::vec2 Texture::get_Size()
{
	return v2_Size;
}