#pragma once

#include <string>
#include <vector>
#include "Engine/Astar/Node.h"

class Node_Graph
{
private:
	std::vector<Node*> nodes;
	Node * GetClosestNode(glm::vec3 position, std::vector<Node*>::iterator i, std::vector<Node*>::iterator end);
public:
	//add a node to the graph
	void AddNode(glm::vec3 nodePosition);
	//get the node closest to a given point
	Node* GetClosestNode(glm::vec3 position);
	//get the closest point that lies on a path connecting two nodes
	glm::vec3 GetClosestPathPoint(glm::vec3 position);
	//return the number of nodes in the graph
	const unsigned int NumNodes();
	//get a node by index
	Node* GetNode(unsigned int i);
	//find a path from point a to point b
	std::vector<glm::vec3> GetPath(glm::vec3 start, glm::vec3 goal);
	//--todo/unimplemented-- load from file, always returns false
	bool LoadFromFile(std::string filePath);
	~Node_Graph();
};