#include "Engine/Astar/Node_Graph.h"
#include "Engine/Astar/AStarPathfinder.h"
#include <glm/gtx/norm.hpp>

Node_Graph::~Node_Graph()
{
	//clean up memory used
	for (size_t i = 0; i < nodes.size(); i++)
	{
		delete nodes[i];
	}
}

void Node_Graph::AddNode(glm::vec3 nodePosition)
{
	nodes.push_back(new Node(nodePosition));
}

const unsigned int Node_Graph::NumNodes()
{
	return nodes.size();
}

Node * Node_Graph::GetNode(unsigned int i)
{
	return nodes.at(i);
}

Node * Node_Graph::GetClosestNode(glm::vec3 position, std::vector<Node*>::iterator i, std::vector<Node*>::iterator end)
{
	//set the first node in the list to the initial best node
	Node * best = (*i++);
	float dist = glm::length2(position - best->GetPosition());
	float bestDist = dist;

	//for all other nodes check if they are closer than current best
	for (; i < end; i++)
	{
		dist = glm::length2(position - (*i)->GetPosition());
		if (dist < bestDist)
		{
			best = (*i);
			bestDist = dist;
		}
	}

	//return closest node
	return best;
}

Node * Node_Graph::GetClosestNode(glm::vec3 position)
{
	//no nodes
	if (nodes.size() == 0) { return nullptr; }
	return GetClosestNode(position, nodes.begin(), nodes.end());
}

glm::vec3 Node_Graph::GetClosestPathPoint(glm::vec3 position)
{
	Node * node1 = GetClosestNode(position);
	if (nodes.size() == 1) { return node1->GetPosition(); }

	//get second closest node
	Node * node2 = GetClosestNode(position, node1->GetNeighbourBegin(), node1->GetNeighbourEnd());

	//get closest point on line connecting node1->node2
	glm::vec3 toPoint = position - node1->GetPosition();
	glm::vec3 line = glm::normalize(node2->GetPosition() - node1->GetPosition());
	//dot product(toPoint,line) = distance to closest point on line (extended infinitely)
	float dotProduct = glm::dot(toPoint, line);

	//if dot product is 0 then position is perpendicular to node1 (angle(position,node1,node2) == 90)
	//if the dot product is less than 0 then the closest point is not on the line (angle > 90)
	//in these cases closest point is node1
	if (dotProduct <= 0) { return node1->GetPosition(); }

	//get projected point
	//glm::vec3 projectedPoint = dotProduct * line;
	////if node1->projectedPoint > node1->node2 line is out of bounds and closest point is node2
	//// but this should never be the case of node2 would be the closest point and not node1
	//if (glm::length2(projectedPoint) > glm::length2(line)) { return node2->GetPosition(); }

	//return closest point
	return (dotProduct * line) + node1->GetPosition();
}

std::vector<glm::vec3> Node_Graph::GetPath(glm::vec3 start, glm::vec3 goal)
{
	std::vector<glm::vec3> path;
	Node * startNode = GetClosestNode(start);
	Node * goalNode = GetClosestNode(goal);

	if (startNode != goalNode)
	{
		AStarPathfinder PathFinder(startNode, goalNode);
		if (PathFinder.PathFound())
		{
			//head to connecting path first
			glm::vec3 point = GetClosestPathPoint(start);
			if (point != PathFinder.GetPath()[0])
			{
				path.push_back(point);
			}

			//add rest of path from pathfinder
			path.insert(path.end(), PathFinder.GetPath().begin(), PathFinder.GetPath().end());

			//add goal to path
			if (goal != PathFinder.GetPath().back())
			{
				path.push_back(goal);
			}

			return path;
		}
	}

	//path not found, return dumb path from given start to goal positions
	path.push_back(start);
	path.push_back(goal);
	return path;
}

bool Node_Graph::LoadFromFile(std::string filePath)
{
	//
	return false;
}