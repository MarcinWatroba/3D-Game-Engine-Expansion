#include <Engine\GUI\Font.h>
#include <TinyXML2\tinyxml2.h>
#include <iostream>
#include <string>
#include <Engine/GUI/Glyph.h>
#include <Engine\Creators\Texture.h>

Font::Font(const char* pc_FileName_In, Texture * p_Texture_In)
{
	p_Texture = p_Texture_In;

	tinyxml2::XMLDocument object_File;
	object_File.LoadFile(pc_FileName_In);
	tinyxml2::XMLElement* body = object_File.FirstChildElement("font");

	std::cout << "Adding new font" << "\n";

	for (tinyxml2::XMLElement* it = body->FirstChildElement("chars")->FirstChildElement("char"); it != nullptr; it = it->NextSiblingElement("char"))
	{
		//Extract data
		char c_ID = std::atoi(it->Attribute("id"));
		glm::vec2 v2_Position = glm::vec2(std::strtof(it->Attribute("x"), nullptr), std::strtof(it->Attribute("y"), nullptr));
		glm::vec2 v2_Length = glm::vec2(std::strtof(it->Attribute("width"), nullptr), std::strtof(it->Attribute("height"), nullptr));
		glm::vec2 v2_Offset = glm::vec2(std::strtof(it->Attribute("xoffset"), nullptr), std::strtof(it->Attribute("yoffset"), nullptr));
		int i_XAdvance = std::atoi(it->Attribute("xadvance"));

		mcop_Glyphs.insert(std::pair<char, Glyph*>(c_ID, new Glyph(v2_Position, p_Texture->get_Size(), v2_Length, glm::vec3(1.f, 1.f, 1.f), c_ID, v2_Offset, i_XAdvance)));
	}
}

void Font::clean_Up()
{
	for (auto const& pair : mcop_Glyphs) delete pair.second;
	mcop_Glyphs.clear();
}

Glyph* Font::get_Letter(char c_ID_In)
{
	return mcop_Glyphs.find(c_ID_In)->second;
}

Texture * Font::get_Texture()
{
	return p_Texture;
}

