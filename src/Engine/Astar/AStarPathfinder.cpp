#include "Engine/Astar/AStarPathfinder.h"

AStarPathfinder::AStarPathfinder(Node * start, Node * goal)
{
	FindPath(start, goal);
	ResetVisited();
}

std::vector<glm::vec3> AStarPathfinder::GetPath()
{
	return path;
}

bool AStarPathfinder::PathFound()
{
	return path.size() != 0;
}

void AStarPathfinder::ResetVisited()
{
	//reset all the nodes that were visited
	for (size_t i = 0; i < visitedNodes.size(); i++)
	{
		visitedNodes[i]->Reset();
	}
	//clear the list
	visitedNodes.clear();
}

void AStarPathfinder::FindPath(Node * start, Node * goal)
{
	//set goal node to start node and vice versa,
	// will search from goal to start so path is in correct order
	goalNode = start;
	currentNode = goal;
	//open start node
	currentNode->Open(nullptr, goalNode);
	numOpenNodes = 1;
	//add start node to visited node list
	visitedNodes.push_back(currentNode);

	//iterate until open nodes are exhausted (no path) or path was found
	while (numOpenNodes != 0)
	{
		//set the current node to the one with the lowest heuristic
		for (size_t i = 0; i < visitedNodes.size(); i++)
		{
			//skip if node is closed
			if (!visitedNodes[i]->IsOpen()) { continue; }
			//is current node is closed then set it to the first open node
			if (!currentNode->IsOpen())
			{
				currentNode = visitedNodes[i];
				continue;
			}
			//set a new current node if this node has a lower heuristic cost
			if (visitedNodes[i]->GetCombinedHeuristic() < currentNode->GetCombinedHeuristic())
			{
				currentNode = visitedNodes[i];
			}
		}

		//close the current node
		currentNode->Close();
		numOpenNodes--;

		//if current node is the goal node, a valid path was found
		if (currentNode == goalNode)
		{
			//construct path
			while (currentNode != nullptr)
			{
				path.push_back(currentNode->GetPosition());
				currentNode = currentNode->GetParent();
			}
			//return as nothing else needs to be done
			return;
		}

		//Check neighbour nodes
		for (size_t i = 0; i < currentNode->NumNeighbours(); i++)
		{
			neighbour = currentNode->GetNeighbour(i);
			//if neighbour has not yet been checked
			if (!neighbour->IsChecked())
			{
				//open the node
				neighbour->Open(currentNode, goalNode);
				numOpenNodes++;
				//add to visited node list
				visitedNodes.push_back(neighbour);
			}
			//if neighbour node has been visited and is still open
			else if (neighbour->IsOpen())
			{
				//if current path heuristic is greater than the path heuristic using the current node as the parent
				if (neighbour->GetPathHeuristic() > currentNode->GetPathHeuristic() + glm::length(neighbour->GetPosition() - currentNode->GetPosition()))
				{
					//re-open node with the current node as the parent node
					neighbour->Open(currentNode, goalNode);
				}
			}
		}
	}
}