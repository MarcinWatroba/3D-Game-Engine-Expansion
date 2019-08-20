#include "Engine\GUI\Tile.h"
#include <Engine\Creators\Texture.h>
#include <Engine\Mesh\Mesh_2D.h>
#include <Engine\Component\Transform_2D.h>
#include <Engine\Component\RenderComp_2D.h>
#include <glad\glad.h>

Tile::Tile(const Tile & tile) : GameObject_2D(tile)
{
}

Tile::Tile(int i_ObjectID_In, std::string s_Text_In, Mesh* p_Mesh_In, Texture* p_Texture_In, glm::vec2 v2_Position_In, glm::vec2 v2_Size_In, float f_Angle_In, float f_Layer_In)
{
	set_Name("Tile");
	add_Component("Mesh_2D", p_Mesh_In);
	add_Component("Transform_2D", new Transform_2D());
	add_Component("RenderComp_2D", new RenderComp_2D());
	add_Texture("Texture_Atlas", p_Texture_In);
	set_Position(v2_Position_In);
	set_Origin(glm::vec2(0.f, 0.f));
	set_Size(v2_Size_In);
	set_Angle(f_Angle_In);
	set_Layer(f_Layer_In);
	set_Tiles(glm::vec2(1.f, 1.f));
	set_Tag("GUI");
	set_RenderStatus(true);
	set_ObjectID(i_ObjectID_In);
}

void Tile::update()
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update();
}

void Tile::render(Shader * p_Shader_In)
{
	static_cast<Transform_2D*>(mipo_Components.find("Transform_2D")->second)->update_Shader(p_Shader_In);
	if (b_RenderStatus) static_cast<RenderComp_2D*>(mipo_Components.find("RenderComp_2D")->second)->render(GL_TEXTURE_2D, GL_TRIANGLES, p_Shader_In);
}
