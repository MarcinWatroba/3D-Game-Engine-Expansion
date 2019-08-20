#pragma once

#include "Engine/Astar/Node_Graph.h"
#include "Engine/Game_objects/GameObject_3D.h"
#include <Engine\Loaders\Loader.h>

namespace ccs//character controller states
{
	enum AIController_States
	{
		Idle,
		Patrol,
		Attack,
		Dialogue
	};
}

struct AIController_Data
{
	//maximum distance to begin switch to chasing and attacking the player
	const float startChaseDistance = 10.0f;
	//distance to stop chasing the player
	const float stopChaseDistance = 15.0f;
	//maximum distance to use character attack
	const float attackRange = 5.0f;
	//character turn speed, degrees per seconds
	const float dps = 360.0f;
	//time step
	float deltaTime;
	//patrol path pointer
	std::vector<glm::vec3>* path;
	//scene resources
	Loader * loader;
	//AI map
	Node_Graph * AI_Map;
	//pointer to this AI character
	GameObject_3D * character;
	//pointer to the player character
	GameObject_3D * player;
};