#include <Editor\Objects\Arrow_3D.h>
#include <Engine\Mesh\Mesh_3D.h>
#include <Engine\Component\Transform_3D.h>
#include <Engine\Component\RenderComp_3D.h>
#include <Engine\Creators\Texture.h>

Arrow_3D::Arrow_3D(int i_ID_In, std::string s_Name_In, Mesh_3D * p_Mesh_In, glm::quat v2_Rotation_In, Texture * p_Diffuse_In, Texture * p_Specular_In)
{
	set_ObjectID(i_ID_In);
	set_Name(s_Name_In);
	add_Component("Mesh_3D", p_Mesh_In);
	add_Component("Transform_3D", new Transform_3D());
	add_Component("RenderComp_3D", new RenderComp_3D());
	set_Position(glm::vec3(0.f, 0.f, 0.f));
	set_Origin(glm::vec3(0.f, 0.f, 0.f));
	set_Rotation(v2_Rotation_In);
	set_Scale(glm::vec3(1.f, 1.f, 1.f));
	add_Texture("Diffuse_Map", p_Diffuse_In);
	add_Texture("Specular_Map", p_Specular_In);
	set_Tiles(glm::vec2(1.f, 1.f));
	set_Shininess(1.f);
	set_Tag("Arrow");
	set_RenderStatus(false);
}
