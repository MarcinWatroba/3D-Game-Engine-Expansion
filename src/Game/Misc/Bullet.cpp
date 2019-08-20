#include <Game\Misc\Bullet.h>
#include <Engine\Component\RenderComp_3D.h>
#include <Engine\Component\Transform_3D.h>
#include <Engine\Component\Respond_Movement.h>
#include <Engine\Component\BoxCollider_3D.h>
#include <Engine\Component\RigidBody.h>


Bullet::Bullet(std::string s_Name_In, Mesh_3D * p_Mesh_In, GameObject_3D * parentObj, Texture * diffuse, Texture * specular)
{
	
	set_Name(s_Name_In);
	add_Component("Mesh_3D", p_Mesh_In);
	add_Component("Transform_3D", new Transform_3D());
	add_Component("RenderComp_3D", new RenderComp_3D());
	add_Component("Respond_Movement", new Respond_Movement());
	add_Component("BoxCollider_3D", new BoxCollider_3D());
	static_cast<BoxCollider_3D*>(mipo_Components.find("BoxCollider_3D")->second)->setUpBox(&static_cast<Mesh_3D*>(mipo_Components.find("Mesh_3D")->second)->getMinVert(), &static_cast<Mesh_3D*>(mipo_Components.find("Mesh_3D")->second)->getMaxVert());
	add_Component("RigidBody", new RigidBody());
	static_cast<RigidBody*>(mipo_Components.find("RigidBody")->second)->setValues(5, true, get_Position());
	set_Position(glm::vec3(parentObj->get_Position().x, parentObj->get_Position().y + 2, parentObj->get_Position().z));
	set_Rotation(parentObj->get_Rotation());
	set_Origin(glm::vec3(0.f, 0.f, 0.f));
	set_Scale(glm::vec3(0.5f, 0.5f, 1.f));
	add_Texture("Diffuse_Map", diffuse);
	add_Texture("Specular_Map", specular);
	set_Tiles(glm::vec2(1.f, 1.f));
	set_Shininess(1.f);
	set_Tag("Bullet");
	set_RenderStatus(false);
}
