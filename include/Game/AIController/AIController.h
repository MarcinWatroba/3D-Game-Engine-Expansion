#pragma once

#include "Engine/FSM/FSM.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Component/Component.h"
#include "Engine/Game_Objects/GameObject_3D.h"
#include "Game/AIController/AIController_Data.h"

class AIController : public FSM<AIController_Data>, public Component
{
private:
	std::vector<FSM_State<AIController_Data>*> stateList;
public:
	void Update();
	std::string get_Type();
	AIController(GameObject_3D * character);
	~AIController();
};