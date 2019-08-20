#include <Engine\Game_Objects\GameObject_3D.h>
#include <Engine/Component/RenderComp_3D.h>
#include <Engine/Component/Transform_3D.h>
#include <Engine/Component/BoxCollider_3D.h>
#include <Engine/Component/RigidBody.h>
#include <Engine\Component\Respond_Movement.h>
#include <Engine\Component\Character.h>
#include <Engine\Creators\Texture.h>
#include <Engine/Mesh/Mesh_3D.h>
#include <Engine\Audio\Sound.h>
#include <Game\Misc\Bullet.h>
#include <Game\AIController\AIController.h>
#include <glad\glad.h>
#include <iostream>

glm::mat4 GameObject_3D::get_ParentMatrix()
{
	return static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->get_ModelMatrix();
}

GameObject_3D::GameObject_3D()
{
	b_RenderStatus = true;
	count = fireRate;
}

GameObject_3D::GameObject_3D(const GameObject_3D & p_NewObject_In) : Game_Object(p_NewObject_In)
{
	auto transform_3D = static_cast<Transform_3D*>(p_NewObject_In.mipo_Components.at("Transform_3D"));
	add_Component("Transform_3D", new Transform_3D(*transform_3D));
	
	if (p_NewObject_In.vs_ChildrenNames.empty())
	{
		auto mesh_3D = static_cast<Mesh_3D*>(p_NewObject_In.mipo_Components.at("Mesh_3D"));
		add_Component("Mesh_3D", mesh_3D);
	}
	
	for (auto const& pair : p_NewObject_In.mipo_Components)
	{
		if (pair.second->get_Type() == "RenderComp_3D")
		{
			auto renderComp_3D = static_cast<RenderComp_3D*>(pair.second);
			add_Component("RenderComp_3D", new RenderComp_3D(*renderComp_3D));
		}
		else if (pair.second->get_Type() == "Character")
		{
			auto character = static_cast<Character*>(pair.second);
			add_Component("Character", new Character(*character));
		}
		else if (pair.second->get_Type() == "BoxCollider_3D")
		{
			auto boxCollider_3D = static_cast<BoxCollider_3D*>(pair.second);
			add_Component("BoxCollider_3D", new BoxCollider_3D(*boxCollider_3D));
		}
		else if (pair.second->get_Type() == "RigidBody")
		{
			auto rigidBody_3D = static_cast<RigidBody*>(pair.second);
			add_Component("RigidBody", new RigidBody(*rigidBody_3D));
		}
		else if (pair.second->get_Type() == "Respond_Movement")
		{
			auto respond_Movement = static_cast<Respond_Movement*>(pair.second);
			add_Component("Respond_Movement", new Respond_Movement(*respond_Movement));
		}
		else if (pair.second->get_Type() == "AI_Controller")
		{
			auto AI = static_cast<AIController*>(pair.second);
			add_Component("AI_Controller", new AIController(*AI));
		}
	}
	s_PrefabName = p_NewObject_In.s_PrefabName;
}

void GameObject_3D::add_Component(std::string s_Name_In, Component* p_Component_In)
{
	mipo_Components.insert(std::pair<std::string, Component*>(s_Name_In, p_Component_In));

	if (s_Name_In == "RenderComp_3D")
	{
		auto found_Mesh = mipo_Components.find("Mesh_3D");

		if (found_Mesh == mipo_Components.end()) std::cout << "Mesh not found" << "\n";
		else
		{
			auto found_Render = mipo_Components.find("RenderComp_3D")->second;
			static_cast<RenderComp_3D*>(found_Render)->set_Mesh(static_cast<Mesh_3D*>(found_Mesh->second));
		}
	}
	else if (s_Name_In == "BoxCollider_3D")
	{
		auto found_Mesh = mipo_Components.find("Mesh_3D");

		if (found_Mesh == mipo_Components.end()) std::cout << "Mesh not found" << "\n";
		else
		{
			auto found_Collider = mipo_Components.find("BoxCollider_3D")->second;
			static_cast<BoxCollider_3D*>(found_Collider)->setUpBox(&static_cast<Mesh_3D*>(found_Mesh->second)->getMinVert(), &static_cast<Mesh_3D*>(found_Mesh->second)->getMaxVert());
		}

	}
	else if (s_Name_In == "RigidBody")
	{
		auto found_RigidBody = mipo_Components.find("RigidBody")->second;
		dynamic_cast<RigidBody*>(found_RigidBody)->setValues(1.0f, false, dynamic_cast<Transform_3D*>(mipo_Components.at("Transform_3D"))->get_Position());
	}
	else if (s_Name_In == "Character")
	{
		auto found_Character = mipo_Components.find("Character")->second;
	}
}

void GameObject_3D::add_Texture(std::string s_Name_In, Texture* p_Texture_In)
{
	static_cast<RenderComp_3D*>(mipo_Components.find("RenderComp_3D")->second)->add_Texture(s_Name_In, p_Texture_In);
}

void GameObject_3D::update()
{
	if (po_Parent != nullptr)
	{
		static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->force_Update(); // Update must be forced to update any children
		static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->update(static_cast<GameObject_3D*>(po_Parent)->get_ParentMatrix());
		if (mipo_Components.count("BoxCollider_3D"))
		{
			dynamic_cast<BoxCollider_3D*>(mipo_Components.find("BoxCollider_3D")->second)->updatePos(static_cast<GameObject_3D*>(po_Parent)->get_ParentMatrix());
		}
	}
	else
	{
		static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->update();
		if (mipo_Components.count("BoxCollider_3D"))
		{
			dynamic_cast<BoxCollider_3D*>(mipo_Components.find("BoxCollider_3D")->second)->updatePos(get_ParentMatrix());
		}
	}
	
	shootBullet();
	for (int i = 0; i < (signed)bulletList.size(); i++)
	{
		if (bulletList.empty())
		{
			break;
		}
		bulletList[i]->update();
		if (static_cast<BoxCollider_3D*>(bulletList[i]->get_Components().at("BoxCollider_3D"))->getCollisionCheck())
		{
			delete bulletList[i];
			bulletList.erase(bulletList.begin() + i);
			bulletNumber--;
			i--;
		}
		
	}
}

void GameObject_3D::renderDepth(Shader* p_Shader_In)
{
	static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->update_Shader(p_Shader_In);
	if (b_RenderStatus) static_cast<RenderComp_3D*>(mipo_Components.find("RenderComp_3D")->second)->renderDepth(GL_TEXTURE_2D, GL_TRIANGLES, p_Shader_In);
	else
	{
		static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->update();
	}
}

void GameObject_3D::render(Shader* p_Shader_In)
{
	static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->update_Shader(p_Shader_In);
	if (b_RenderStatus) static_cast<RenderComp_3D*>(mipo_Components.find("RenderComp_3D")->second)->render(GL_TEXTURE_2D, GL_TRIANGLES, p_Shader_In);

	for (unsigned int i = 0; i < bulletList.size(); i++)
	{
		bulletList[i]->render(p_Shader_In);
	}
}

void GameObject_3D::render(Shader* p_Shader_In, bool skybox)
{
	static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->update_Shader(p_Shader_In);
	if (b_RenderStatus) static_cast<RenderComp_3D*>(mipo_Components.find("RenderComp_3D")->second)->render(GL_TEXTURE_2D, GL_TRIANGLES, p_Shader_In, skybox);

	for (unsigned int i = 0; i < bulletList.size(); i++)
	{
		bulletList[i]->render(p_Shader_In);
	}
}

void GameObject_3D::clean_Up()
{
	if (!mipo_Components.empty()) for (auto map : mipo_Components) if(map.first != "Mesh_3D") delete map.second;
	mipo_Components.clear();
}

void GameObject_3D::force_Update()
{
	static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->force_Update();
}

void GameObject_3D::set_Position(glm::vec3 v3_Position_In)
{
	static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->set_Position(v3_Position_In);
}

void GameObject_3D::set_Scale(glm::vec3 v3_Scale_In)
{
	static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->set_Scale(v3_Scale_In);
}

void GameObject_3D::set_Origin(glm::vec3 v3_Origin_In)
{
	static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->set_Origin(v3_Origin_In);
}

void GameObject_3D::set_Rotation(glm::quat quat_In)
{
	static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->set_Rotation(quat_In);
}

void GameObject_3D::update_Rotation(glm::quat quat_In)
{
	static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->update_Rotation(quat_In);
}

glm::vec3 GameObject_3D::get_Position()
{
	return static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->get_Position();
}

glm::vec3 GameObject_3D::get_Origin()
{
	return static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->get_Origin();
}

glm::quat GameObject_3D::get_Rotation()
{
	return static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->get_Rotation();
}

glm::vec3 GameObject_3D::get_Scale()
{
	return static_cast<Transform_3D*>(mipo_Components.find("Transform_3D")->second)->get_Scale();
}

std::vector<GameObject_3D*> GameObject_3D::get_BulletList()
{
	return bulletList;
}

void GameObject_3D::set_Shininess(float f_Shiny_In)
{
	static_cast<RenderComp_3D*>(mipo_Components.find("RenderComp_3D")->second)->set_Shininess(f_Shiny_In);
}

void GameObject_3D::set_Tiles(glm::vec2 v2_Tiles_In)
{
	static_cast<RenderComp_3D*>(mipo_Components.find("RenderComp_3D")->second)->set_Tiles(v2_Tiles_In);
}
void GameObject_3D::move(glm::vec3 v3_Direction_In, float f_Speed_In)
{
	if (mipo_Components.count("Respond_Movement"))
	{
		auto found_Movement = mipo_Components.at("Respond_Movement");
		bool isPositive;
		if (f_Speed_In > 0)
		{
			isPositive = true;
		}
		else
		{
			isPositive = false;
		}

		if (mipo_Components.count("RigidBody"))
		{
			bool tempCheck = false;
			tempCheck = dynamic_cast<BoxCollider_3D*>(mipo_Components.at("BoxCollider_3D"))->getCollisionCheck();
			if (tempCheck && lastDir == isPositive)
			{
				dynamic_cast<RigidBody*>(mipo_Components.at("RigidBody"))->setForwardForce(0);
			}
			else
			{
				dynamic_cast<RigidBody*>(mipo_Components.at("RigidBody"))->setForwardForce(f_Speed_In);
			}
			dynamic_cast<RigidBody*>(mipo_Components.at("RigidBody"))->update(get_Position());
			dynamic_cast<Respond_Movement*>(found_Movement)->move(this, v3_Direction_In, dynamic_cast<RigidBody*>(mipo_Components.at("RigidBody"))->getVelocity());
		}
		else
		{
			dynamic_cast<Respond_Movement*>(found_Movement)->move(this, v3_Direction_In, f_Speed_In);
		}
		lastDir = isPositive;
	}
}

void GameObject_3D::jump(glm::vec3 v3_Direction_In)
{
	if (mipo_Components.count("Respond_Movement"))
	{
		auto found_Movement = mipo_Components.at("Respond_Movement");
		dynamic_cast<RigidBody*>(mipo_Components.at("RigidBody"))->update(get_Position());
		dynamic_cast<Respond_Movement*>(found_Movement)->move(this, v3_Direction_In, dynamic_cast<RigidBody*>(mipo_Components.at("RigidBody"))->getJumpVelocity());
	}
}

void GameObject_3D::turn(float f_Angle_In, glm::vec3 v3_TurnAxis_In)
{
	if (mipo_Components.count("Respond_Movement"))
	{
		auto found_Movement = mipo_Components.at("Respond_Movement");
		dynamic_cast<Respond_Movement*>(found_Movement)->turn(this, f_Angle_In, v3_TurnAxis_In);
	}
}

void GameObject_3D::setFiring(bool input)
{
	firing = input;
}

void GameObject_3D::createBullet(Bullet* bulletTemplate, Sound* temp_Audio)
{ 
	if (count == fireRate)
	{
		if (static_cast<Character*>(mipo_Components.at("Character"))->getNumberOfBullets() != 0)
		{
			bulletList.push_back(bulletTemplate);
			bulletList[bulletNumber]->set_RenderStatus(true);
			static_cast<Character*>(mipo_Components.at("Character"))->loseBullets();
			bulletNumber++;
			count = 0;
			float e = static_cast<Character*>(mipo_Components.at("Character"))->getNumberOfBullets();
			temp_Audio->Play();
		}
		
	}
	else
	{
		count++;
	}

}
std::string GameObject_3D::get_Type()
{
	return "GameObject_3D";
}

void GameObject_3D::shootBullet()
{
	for (unsigned int i = 0; i < bulletList.size(); i++)
	{
		glm::vec3 temp = static_cast<Transform_3D*>(bulletList[i]->get_Component("Transform_3D"))->get_Forward();
		static_cast<Respond_Movement*>(bulletList[i]->get_Component("Respond_Movement"))->moveAbsolute(bulletList[i], temp, 1);
	}
	if (!bulletList.empty())
	{
		glm::vec3 temp = bulletList[0]->get_Position();
	}
}

void GameObject_3D::resetCount()
{
	count = fireRate;
}
