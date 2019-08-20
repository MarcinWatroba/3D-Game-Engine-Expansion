#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Engine/Astar/Node.h"

class AStarPathfinder
{
private:
	Node * currentNode;
	Node * goalNode;
	Node * neighbour;
	int numOpenNodes;
	std::vector<Node *> visitedNodes;
	std::vector<glm::vec3> path;

	void ResetVisited();
	void FindPath(Node * start, Node * goal);

public:
	std::vector<glm::vec3> GetPath();
	bool PathFound();
	AStarPathfinder(Node * start, Node * goal);
};