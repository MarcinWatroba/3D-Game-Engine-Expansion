#include "Engine/Astar/Node.h"

Node::Node(glm::vec3 pos) :
	position(pos),
	active(false)
{}

glm::vec3 Node::GetPosition()
{
	return position;
}

Node * Node::GetParent()
{
	return came_from;
}

float Node::GetCombinedHeuristic()
{
	return f;
}

float Node::GetPathHeuristic()
{
	return g;
}

bool Node::IsChecked()
{
	return active;
}

bool Node::IsOpen()
{
	return open;
}

void Node::Open(Node * parent, Node * goal)
{
	//
	active = true;
	open = true;
	came_from = parent;
	//set goal heuristic
	h = glm::length(position - goal->GetPosition());
	//set current path cost heuristic
	if (parent != nullptr)
	{
		g = parent->g + glm::length(position - parent->GetPosition());
	}
	else
	{
		g = 0;
	}
	//set combined heuristic
	f = g + h;
}

void Node::Close()
{
	open = false;
}

void Node::Reset()
{
	Close();
	active = false;
}

unsigned int Node::NumNeighbours()
{
	return neighbours.size();
}

Node * Node::GetNeighbour(unsigned int i)
{
	return neighbours.at(i);
}

void Node::AddNeighbour(Node * neighbour)
{
	neighbours.push_back(neighbour);
}

std::vector<Node*>::iterator Node::GetNeighbourBegin()
{
	return neighbours.begin();
}
std::vector<Node*>::iterator Node::GetNeighbourEnd()
{
	return neighbours.end();
}