#include <Engine/Component/RenderComp.h>
#include <Engine/Creators/Texture.h>

RenderComp::RenderComp() {};
RenderComp::~RenderComp() {};
RenderComp::RenderComp(const RenderComp & p_NewComp_In)
{
	v3_Colour = p_NewComp_In.v3_Colour;
	v2_Tiling = p_NewComp_In.v2_Tiling;
	p_Mesh = p_NewComp_In.p_Mesh;
	mso_Textures = p_NewComp_In.mso_Textures;
}
std::string RenderComp::get_Type()
{
	return "RenderComp";
}

glm::vec2 RenderComp::get_Tiles()
{
	return v2_Tiling;
}

void RenderComp::set_Tiles(glm::vec2 v2_Tiles_In)
{
	v2_Tiling = v2_Tiles_In;
}

void RenderComp::set_Colour(glm::vec3 rgb_Colour_In)
{
	v3_Colour = rgb_Colour_In;
}

glm::vec3 RenderComp::get_Colour()
{
	return v3_Colour;
}

void RenderComp::set_Mesh(Mesh* p_Mesh_In)
{
	p_Mesh = p_Mesh_In;
}

void RenderComp::add_Texture(std::string s_Name_In, Texture* p_Texture_In)
{
	mso_Textures.insert(std::pair<std::string, Texture*>(s_Name_In, p_Texture_In));
}

Texture* RenderComp::get_Texture(std::string s_Name_In)
{
	auto found = mso_Textures.find(s_Name_In)->second;
	return found;
}

