#include "Game/AIController/State_Patrol.h"
#include "Game/AIController/AIController_Data.h"
#include <Engine/Component/Transform_3D.h>
#include <Engine/Component/Respond_Movement.h>
#include <iostream>

State_Patrol::State_Patrol(const int setStateID) :
	FSM_State<AIController_Data>(setStateID)
{}

void State_Patrol::OnEnter()
{
	//std::cout << "Patrol_enter\n";
	pathStep = 0;
	timer = 0.0f;
	//
	wait = 1.0f;
}

fsm::FSM_Command State_Patrol::OnRun()
{
	if (data->path->size() == 0) { return fsm::Continue; }
	//
	Respond_Movement* movement = static_cast<Respond_Movement*>(data->character->get_Component("Respond_Movement"));

	//check if player is close enough to chase
	if (data->player != nullptr && movement->withinRange(data->character, data->player->get_Position(), data->startChaseDistance))
	{
		nextStateID = ccs::Attack;
		return fsm::Switch;
	}

	//move to point
	bool atPoint = movement->moveToPoint(data->character, (*data->path)[pathStep], 10.0f * data->deltaTime, data->dps * data->deltaTime);

	//wait for the given time
	if (atPoint)
	{

		timer += data->deltaTime;
		if (timer > wait)//wait[pathStep])
		{
			//reset timer and increment path step
			timer = 0.0f;
			pathStep++;
			if (pathStep == data->path->size()) { pathStep = 0; }
		}
	}

	//
	return fsm::Continue;
}

void State_Patrol::OnExit()
{
	//std::cout << "Patrol_exit\n";
}