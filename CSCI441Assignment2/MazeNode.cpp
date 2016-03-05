#include "MazeNode.h"

int MazeNode::getXpos() const
{
	return x;
}

int MazeNode::getYpos() const
{
	return y;
}

void MazeNode::setVisited()
{
	visited = true;
}

void MazeNode::clearVisited()
{
	visited = false;
}

bool MazeNode::isVisited()
{
	return visited;
}

MazeNode::MazeNode() : StaticNode(4)
{
	x = 0;
	y = 0;
	visited = false;
}

MazeNode::MazeNode(int x, int y) : StaticNode(4)
{
	this->x = x;
	this->y = y;
	visited = false;
}

MazeNode::MazeNode(const MazeNode& node) : StaticNode(*this)
{
	visited = node.visited;
}


MazeNode::~MazeNode()
{
	destroyNode();
}

MazeNode* MazeNode::cloneNode() const
{
	return new MazeNode(*this);
}

int MazeNode::getConnectBackIndex(int pos)
{
	return (pos + (this->getNumOfConnections()/2)) % this->getNumOfConnections();
}
