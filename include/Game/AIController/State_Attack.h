#pragma once

#include "Game/AIController/AIController_Data.h"
#include "Engine/FSM/FSM_State.h"

class State_Attack : public FSM_State<AIController_Data>
{
protected:
	void OnEnter();
	fsm::FSM_Command OnRun();
	void OnExit();
public:
	State_Attack(const int setStateID);
};