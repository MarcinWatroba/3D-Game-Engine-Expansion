#pragma once

class Glyph;
class Texture;
#include <map>

class Font
{
private:
	std::map<char, Glyph*> mcop_Glyphs;
	Texture* p_Texture;
public:
	Font() {};
	Font(const char* pc_FileName_In, Texture* p_Texture_In);

	void clean_Up();
	Glyph* get_Letter(char c_ID_In);
	Texture* get_Texture();
};