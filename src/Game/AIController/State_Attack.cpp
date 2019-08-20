#include "Game/AIController/State_Attack.h"
#include "Game/AIController/AIController_Data.h"
#include <Engine/Component/Transform_3D.h>
#include <Engine/Component/RigidBody.h>
#include <iostream>
#include <glm/gtx/norm.hpp>
#include <Engine/Component/Respond_Movement.h>
#include <Game\Misc\Bullet.h>

State_Attack::State_Attack(const int setStateID) :
	FSM_State<AIController_Data>(setStateID)
{}

void State_Attack::OnEnter()
{
	//std::cout << "Attack_enter\n";
}

fsm::FSM_Command State_Attack::OnRun()
{
	//player destroyed
	if (data->player == nullptr)
	{
		return fsm::Finish;
	}

	//
	Respond_Movement* movement = static_cast<Respond_Movement*>(data->character->get_Component("Respond_Movement"));
	float distSqr = glm::length2(data->player->get_Position() - data->character->get_Position());

	//check if player has gone too far away
	if (distSqr > data->stopChaseDistance*data->stopChaseDistance)
	{
		return fsm::Finish;
	}

	//if too far away move close enough to attack
	if (distSqr > data->attackRange*data->attackRange)
	{
		movement->moveToPoint(data->character, data->player->get_Position(), 10.0f * data->deltaTime, data->dps * data->deltaTime);
	}

	//in attack range
	else
	{
		//back off if too close
		if (distSqr < 5.0f)
		{
			movement->move(data->character, glm::vec3(0,0,-1), 10.0f * data->deltaTime);
		}
		//shoot at player
		bool facingTarget = movement->facePoint(data->character, data->player->get_Position(), data->dps * data->deltaTime);
		if (facingTarget)
		{
			data->character->createBullet(new Bullet("Bullet", (Mesh_3D*)data->loader->get_Mesh("7"), data->character, data->loader->get_Texture("24"), data->loader->get_Texture("7")), data->loader->get_Sound("2"));
		}
	}

	//
	return fsm::Continue;
}

void State_Attack::OnExit()
{
	//std::cout << "Attack_exit\n";
}