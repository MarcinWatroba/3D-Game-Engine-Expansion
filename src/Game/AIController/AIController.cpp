#include "Game/AIController/AIController.h"
#include "Game/AIController/State_Idle.h"
#include "Game/AIController/State_Patrol.h"
#include "Game/AIController/State_Attack.h"
#include <Engine/Component/Character.h>

AIController::AIController(GameObject_3D * character) :
	FSM(&stateList)
{
	stateList.push_back(new State_Idle(ccs::Idle));
	stateList.push_back(new State_Patrol(ccs::Patrol));
	stateList.push_back(new State_Attack(ccs::Attack));
	InitializeToState(ccs::Idle);
	data.character = nullptr;
	data.path = nullptr;
}

std::string AIController::get_Type()
{
	return "AI_Controller";
}

AIController::~AIController()
{
	for (unsigned int i = 0; i < stateList.size(); i++)
	{
		delete stateList[i];
	}
}

void AIController::Update()
{
	if (data.character == nullptr)
	{
		return;
	}
	if (data.path == nullptr)
	{
		Character* characterComp = static_cast<Character*>(data.character->get_Component("Character"));
		if (characterComp != nullptr) { data.path = characterComp->getPath(); }
	}
	Run();
}
