#pragma once

#include "Game/AIController/AIController_Data.h"
#include "Engine/FSM/FSM_State.h"
#include <vector>
#include <glm/glm.hpp>

class State_Patrol : public FSM_State<AIController_Data>
{
private:
	//std::vector<float> wait;//time to wait at each point
	float wait;
	unsigned int pathStep;//current step along patrol path
	float timer;//
protected:
	void OnEnter();
	fsm::FSM_Command OnRun();
	void OnExit();
public:
	State_Patrol(const int setStateID);
};