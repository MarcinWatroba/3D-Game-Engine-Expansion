#include <Engine\Loaders\Loader.h>
#include <iostream>
#include <glad\glad.h>
#include <string>

void Loader::start()
{
	std::cout << "Parsing XML resource file..." << "\n";
	ParseXML_Resources("assets/Resources.xml");
	std::cout << "Pasrsing completed..." << "\n";
}

Mesh* Loader::get_Mesh(std::string s_Name_In)
{
	return mipo_Meshes.find(s_Name_In)->second;
}

Font* Loader::get_Font(std::string s_Name_In)
{
	return mipo_Fonts.find(s_Name_In)->second;
}

Shader* Loader::get_Shader(std::string s_Name_In)
{
	return mipo_Shaders.find(s_Name_In)->second;
}

Texture* Loader::get_Texture(std::string s_Name_In)
{
	return mipo_TextureFiles.find(s_Name_In)->second;
}

Sound* Loader::get_Sound(std::string s_Name_In)
{
	return snd_Audio.find(s_Name_In)->second;
}

void Loader::clean_Up()
{
	//Delete every memory
	for (const auto& pair : mipo_Meshes) delete pair.second;
	for (const auto& pair : mipo_Meshes3D) delete pair.second;
	for (const auto& pair : mipo_MeshesInstanced) delete pair.second;
	for (const auto& pair : mipo_TextureFiles) delete pair.second;
	for (const auto& pair : mipo_Shaders) delete pair.second;
	for (const auto& pair : snd_Audio) delete pair.second;
}

void Loader::ParseXML_Resources(const char* pc_FileName)
{
	tinyxml2::XMLDocument resources_File; // Create a file template
	resources_File.LoadFile(pc_FileName); // Load the file
	tinyxml2::XMLElement* po_Body = resources_File.FirstChildElement("resources"); // Start from <models>

	//Obj files first
	std::string obj_FileLoc = po_Body->FirstChildElement("object_File_Location")->GetText();
	for (tinyxml2::XMLElement* i = po_Body->FirstChildElement("object_Files")->FirstChildElement("new_File"); i != nullptr; i = i->NextSiblingElement("new_File"))
	{
		//Create variables
		std::string i_ID = i->Attribute("ID");
		std::string s_Name = obj_FileLoc + i->Attribute("name");
		int i_DrawMethod = std::atoi(i->Attribute("i_DrawMethod"));

		mipo_Meshes.insert(std::pair<std::string, Mesh_3D*>(i_ID, new Mesh_3D(s_Name.c_str(), i_DrawMethod, i_ID)));
	}

	for (tinyxml2::XMLElement* i = po_Body->FirstChildElement("particle_Files")->FirstChildElement("new_File"); i != nullptr; i = i->NextSiblingElement("new_File"))
	{
		//Create variables
		std::string i_ID = i->Attribute("ID");
		std::string s_Name = obj_FileLoc + i->Attribute("name");
		int i_DrawMethod = std::atoi(i->Attribute("i_DrawMethod"));

		mipo_Meshes.insert(std::pair<std::string, Mesh_Instanced*>(i_ID, new Mesh_Instanced(s_Name.c_str(), i_DrawMethod)));
	}

	std::string texture_FileLoc = po_Body->FirstChildElement("texture_File_Location")->GetText();
	for (tinyxml2::XMLElement* i = po_Body->FirstChildElement("texture_Files")->FirstChildElement("new_File"); i != nullptr; i = i->NextSiblingElement("new_File"))
	{
		//Create variables
		std::string i_ID = i->Attribute("ID");
		std::string s_Name = texture_FileLoc + i->Attribute("name");
		int i_TextureMode = std::atoi(i->Attribute("texture_Mode"));
		int i_WrappingS = std::atoi(i->Attribute("wrapping_S"));
		int i_WrappingT = std::atoi(i->Attribute("wrapping_T"));
		int i_FilterMin = std::atoi(i->Attribute("filter_Min"));
		int i_FilterMag = std::atoi(i->Attribute("filter_Mag"));
		int i_SoilColourLoad = std::atoi(i->Attribute("soil_Colour_Load"));
		int i_OpenGLColour = std::atoi(i->Attribute("openGL_Colour"));
		bool i_GenMipmaps = (std::atoi(i->Attribute("gen_Mipmaps")) != 0);

		//Add texture
		mipo_TextureFiles.insert(std::make_pair(i_ID, new Texture(s_Name.c_str(), i_TextureMode, i_WrappingS, i_WrappingT, i_FilterMin, i_FilterMag, i_SoilColourLoad, i_OpenGLColour, i_GenMipmaps, i_ID)));
	}

	for (tinyxml2::XMLElement* i = po_Body->FirstChildElement("texture_Skybox")->FirstChildElement("new_File"); i != nullptr; i = i->NextSiblingElement("new_File"))
	{
		std::string s_Names[6];
		std::string i_ID = i->Attribute("ID");
		s_Names[0] = texture_FileLoc + i->Attribute("name_right");
		s_Names[1] = texture_FileLoc + i->Attribute("name_left");
		s_Names[2] = texture_FileLoc + i->Attribute("name_top");
		s_Names[3] = texture_FileLoc + i->Attribute("name_bottom");
		s_Names[4] = texture_FileLoc + i->Attribute("name_back");
		s_Names[5] = texture_FileLoc + i->Attribute("name_front");
		int i_TextureMode = std::atoi(i->Attribute("texture_Mode"));
		int i_FilterMin = std::atoi(i->Attribute("filter_Min"));
		int i_FilterMag = std::atoi(i->Attribute("filter_Mag"));
		int i_SoilColourLoad = std::atoi(i->Attribute("soil_Colour_Load"));
		int i_OpenGLColour = std::atoi(i->Attribute("openGL_Colour"));

		mipo_TextureFiles.insert(std::make_pair(i_ID, new Texture(s_Names, i_FilterMin, i_FilterMag, i_SoilColourLoad, i_OpenGLColour)));
	}

	std::string shader_FileLoc = po_Body->FirstChildElement("shader_File_Location")->GetText();
	for (tinyxml2::XMLElement* i = po_Body->FirstChildElement("shader_Files")->FirstChildElement("new_File"); i != nullptr; i = i->NextSiblingElement("new_File"))
	{
		//Create variables
		std::string i_ID = i->Attribute("ID");
		std::string s_NameVertex = shader_FileLoc + i->Attribute("vertex");
		std::string s_NameFragment = shader_FileLoc + i->Attribute("fragment");
		std::string s_NameGeometry = i->Attribute("geometry");

		if (s_NameGeometry == "") s_NameGeometry = "";
		else s_NameGeometry = shader_FileLoc + s_NameGeometry;
		
		//Add shader
		mipo_Shaders.insert(std::make_pair(i_ID, new Shader(s_NameVertex.c_str(), s_NameFragment.c_str(), s_NameGeometry.c_str())));
	}

	std::string font_FileLoc = po_Body->FirstChildElement("font_File_Location")->GetText();
	for (tinyxml2::XMLElement* i = po_Body->FirstChildElement("font_Files")->FirstChildElement("new_File"); i != nullptr; i = i->NextSiblingElement("new_File"))
	{
		//Create variables
		std::string s_ID = i->Attribute("ID");
		std::string s_Font = i->Attribute("font");
		std::string s_Directory = font_FileLoc + s_Font;
		std::string s_Texture = i->Attribute("texture");
		
		//Add font
		mipo_Fonts.insert(std::make_pair(s_ID, new Font(s_Directory.c_str(), mipo_TextureFiles.find(s_Texture)->second)));
	}

	for (tinyxml2::XMLElement* i = po_Body->FirstChildElement("meshes_2D")->FirstChildElement("new_File"); i != nullptr; i = i->NextSiblingElement("new_File"))
	{
		//Create variables
		std::string s_ID = i->Attribute("ID");
		glm::vec2 v2_Position = to2DVector(i->Attribute("position"));
		glm::vec2 v2_Size = get_Texture(i->Attribute("texture"))->get_Size();
		glm::vec2 v2_Dimension = to2DVector(i->Attribute("dimensions"));
		glm::vec3 v3_Colour = to3DVector(i->Attribute("colour"));

		//Add font
		mipo_Meshes.insert(std::make_pair(s_ID, new Mesh_2D(v2_Position, v2_Size, v2_Dimension, v3_Colour)));
	}

	std::string audio_FileLoc = po_Body->FirstChildElement("audio_File_Location")->GetText();
	for (tinyxml2::XMLElement* i = po_Body->FirstChildElement("audio_Files")->FirstChildElement("new_File"); i != nullptr; i = i->NextSiblingElement("new_File"))
	{
		// Create variables
		std::string i_ID = i->Attribute("ID");
		std::string s_Name = i->Attribute("name");
		float f_Volume;
		std::string s_Object;
		glm::vec3 v3_Position;
		bool b_loop;
		bool b_threeD;
		bool b_stream;
		f_Volume = atof(i->Attribute("volume"));
		// If Object is not empty then...
		v3_Position = to3DVector(i->Attribute("position"));

		std::string s_temp = i->Attribute("loop");
		b_loop = (s_temp == "true");

		s_temp = i->Attribute("threeD");
		b_threeD = (s_temp == "true");

		s_temp = i->Attribute("stream");
		b_stream = (s_temp == "true");
		// Add Audio
		snd_Audio.insert(std::make_pair(i_ID, new Sound(s_Name.c_str())));

		// Add the sound with the above data
		auto sound = static_cast<Sound*>(snd_Audio.find(i_ID)->second);
		sound->Load(b_threeD, b_loop, b_stream);
		sound->SetPosition(v3_Position);
		sound->SetVolume(f_Volume);
	}
}

glm::vec3 Loader::to3DVector(const char* pc_Vector3D_In)
{
	std::string s_Result;
	int i_DataCounter = 0;
	int i_Length = std::strlen(pc_Vector3D_In);
	glm::vec3 v3_Vector;

	for (int i = 0; i < i_Length; i++)
	{
		switch (pc_Vector3D_In[i])
		{
		case 32: // Empty space
				 //Ignore
			break;

		case 44: // Comma
			i_DataCounter++;

			switch (i_DataCounter)
			{
			case 1:
				v3_Vector.x = std::strtof(s_Result.c_str(), NULL);
				s_Result.clear();
				break;

			case 2:
				v3_Vector.y = std::strtof(s_Result.c_str(), NULL);
				s_Result.clear();
				break;
			}
			break;
			break;

		case 40: // This bracker "(" 
				 //Ignore
			break;

		case 41: // This bracket ")"
			v3_Vector.z = std::strtof(s_Result.c_str(), NULL);
			break;

			//Process
		default:
			s_Result += pc_Vector3D_In[i];
			break;
		}
	}

	return glm::vec3(v3_Vector.x, v3_Vector.y, v3_Vector.z);
}

glm::vec2 Loader::to2DVector(const char* pc_Vector2D_In)
{
	std::string s_Result;
	int i_DataCounter = 0;
	int i_Length = std::strlen(pc_Vector2D_In);
	glm::vec2 v2_Vector;

	for (int i = 0; i < i_Length; i++)
	{
		switch (pc_Vector2D_In[i])
		{
		case 32: // Empty space
				 //Ignore
			break;

		case 44: // Comma
			v2_Vector.x = std::strtof(s_Result.c_str(), nullptr);
			s_Result.clear();
			break;

		case 40: // This bracker "(" 
				 //Ignore
			break;

		case 41: // This bracket ")"
			v2_Vector.y = std::strtof(s_Result.c_str(), nullptr);
			break;

			//Process
		default:
			s_Result += pc_Vector2D_In[i];
			break;
		}
	}

	return glm::vec2(v2_Vector.x, v2_Vector.y);
}