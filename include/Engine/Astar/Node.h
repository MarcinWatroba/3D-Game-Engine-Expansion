#pragma once

#include <glm/glm.hpp>
#include <vector>

class Node
{
private:
	//node position
	glm::vec3 position;
	//list of possible neighbours
	std::vector<Node *> neighbours;
	//which neighbour this node was accessed from
	Node * came_from;
	//A star heuristic costs
	float g, h, f;
	// whether node has been checked and has been given g,h,f costs
	bool active;
	// whether node is open/closed
	bool open;
public:
	//return the node position
	glm::vec3 GetPosition();
	//return the node that this was accessed from
	Node * GetParent();
	//return the total heuristic (f)
	float GetCombinedHeuristic();
	//return the path heuristic (g)
	float GetPathHeuristic();
	//return whether node has been checked yet (active)
	bool IsChecked();
	//return whether node is open or not
	bool IsOpen();
	//open and score the node
	void Open(Node * parent, Node * goal);
	//close the node
	void Close();
	//reset the node
	void Reset();
	//get the number of neighbour nodes
	unsigned int NumNeighbours();
	//get the given neighbour node
	Node * GetNeighbour(unsigned int i);
	//add a neighbour
	void AddNeighbour(Node * neighbour);
	//get neighbour iterators
	std::vector<Node*>::iterator GetNeighbourBegin();
	std::vector<Node*>::iterator GetNeighbourEnd();
	//
	Node(glm::vec3 pos);
};