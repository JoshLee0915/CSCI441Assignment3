#pragma once
#include "StaticNode.h"
#include "DoublyLinkedNode.h"

using abstractNodes::StaticNode;
using abstractNodes::DoublyLinkedNode;

class MazeNode : public StaticNode<MazeNode>, public DoublyLinkedNode<MazeNode>
{
private:
	int x, y;
	bool visited;

public:
	enum direction
	{
		North,
		East,
		South,
		West
	};
	int getXpos() const;
	int getYpos() const;
	void setVisited();
	void clearVisited();
	bool isVisited();

	MazeNode();
	MazeNode(int x, int y);
	MazeNode(const MazeNode& node);
	virtual ~MazeNode();

	MazeNode* cloneNode() const;

private:
	int getConnectBackIndex(int pos);
};

