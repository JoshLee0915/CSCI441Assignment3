#include "Maze.h"

int Maze::getMazeWidth() const
{
	return width;
}

int Maze::getMazeHeight() const
{
	return height;
}

Maze::Maze(int width, int height, bool failSafe)
{
	if (failSafe && (width*height) > 8649)
		throw std::exception("[ERR]Stack overflow possible at this size.\nKeep total nodes below 8649");

	this->width = width;
	this->height = height;
	this->root = new MazeNode();	//default maze node

	buildMaze();
}

Maze::~Maze()
{
	clearVisited(root);
	destroyMaze(root);		
}

void Maze::buildMaze()
{
	MazeNode* curr;
	MazeNode* prev = 0;
	MazeNode* map = new MazeNode();
	MazeNode* row = map;

	srand((unsigned int)time(0));

	// build intial maze
	for (int y = 0; y < height; y++)
	{
		curr = row;
		for (int x = 1; x < width; x++)
		{
			if (prev && x != 0)
			{
				// atach the current node to the one above it
				curr->addNode(prev, MazeNode::North);
				prev = prev->getNode(MazeNode::East);
			}

			curr->addNode(new MazeNode(), MazeNode::East);
			curr = curr->getNode(MazeNode::East);
		}

		if (y < height-1)
		{
			row->addNode(new MazeNode(), MazeNode::South);
			prev = row;
			row = row->getNode(MazeNode::South);
		}
	}

	// generate the maze
	rndDpthFrstGen(*map,*root);

	// clean up
	destroyBaseMap(map, map);
	map = 0;
}

string Maze::dispayMaze()
{
	string mazeDsp = "";
	string* maze = new string[height*3];
	for (int y = 0; y < height*3;y++)
	{
		maze[y] = "";

		// populate the string
		for (int x = 0; x < width * 3;x++)
			maze[y] += "#";

		maze[y] += "\n";
	}

	clearVisited(root);
	buildCell(*root, maze);

	// append the lines into one string
	for (int y = 0; y < height * 3; y++)
		mazeDsp += maze[y];

	delete[] maze;
	maze = 0;

	return mazeDsp;
}

void Maze::rndDpthFrstGen(MazeNode& map, MazeNode& maze)
{
	int neighbor = 0;
	// first set the current node to visited
	map.setVisited();

	// get a random neighbor while there are unvisited neighbors
	while (containsUnvisited(map))
	{
		// randomly select a neighbor
		neighbor = rand() % map.getNumOfConnections();

		if (map.getNode(neighbor) != 0 && !(map.getNode(neighbor)->isVisited()))
		{
			int xOffset = 0;
			int yOffset = 0;

			// decide on the offset
			switch (neighbor)
			{
			case MazeNode::North:
				yOffset = -1;
				break;
			case MazeNode::East:
				xOffset = 1;
				break;
			case MazeNode::South:
				yOffset = 1;
				break;
			case MazeNode::West:
				xOffset = -1;
				break;
			}
			// add the unvisited node to the path and go on
			maze.addNode(new MazeNode((maze.getXpos()+xOffset), (maze.getYpos()+yOffset)), neighbor);
			rndDpthFrstGen(*map.getNode(neighbor), *maze.getNode(neighbor));
		}
	}
}

void Maze::destroyBaseMap(MazeNode* node, MazeNode* start)
{
	if (node == 0)
		return;

	destroyBaseMap(node->getNode(MazeNode::East), start);

	if (node == start)
		destroyBaseMap(node->getNode(MazeNode::South), node->getNode(MazeNode::South));
		
	delete node;
	node = 0;

}

void Maze::destroyMaze(MazeNode * node)
{
	if (node == 0)
		return;

	node->setVisited();

	for (int index = 0; index < node->getNumOfConnections(); index++)
	{
		if (node->getNode(index) != 0 && !(node->getNode(index)->isVisited()))
			destroyMaze(node->getNode(index));
	}

	delete node;
	node = 0;
}

void Maze::clearVisited(MazeNode * node)
{
	if (node == 0)
		return;

	// clear the current node
	node->clearVisited();

	// traverse all visited node
	for (int index = 0; index < node->getNumOfConnections(); index++)
	{
		if (node->getNode(index) != 0 && node->getNode(index)->isVisited())
			clearVisited(node->getNode(index));
	}
}

void Maze::buildCell(MazeNode & node, string * maze)
{
	// calc the center
	int xCenter = (node.getXpos() * 3) + 1;
	int yCenter = (node.getYpos() * 3) + 1;

	// set the current node as visited
	node.setVisited();

	// clear the center
	maze[yCenter][xCenter] = ' ';

	// draw the cell
	for (int index = 0; index < node.getNumOfConnections(); index++)
	{
		// check if there is a wall or not
		if (node.getNode(index) != 0)
		{
			int xOffset = 0;
			int yOffset = 0;

			// find the correct wall and open it
			switch (index)
			{
			case MazeNode::North:
				yOffset = -1;
				break;
			case MazeNode::East:
				xOffset = 1;
				break;
			case MazeNode::South:
				yOffset = 1;
				break;
			case MazeNode::West:
				xOffset = -1;
				break;
			}

			maze[yCenter + yOffset][xCenter + xOffset] = ' ';

			// move to the next cell
			if(!node.getNode(index)->isVisited())
				buildCell(*node.getNode(index), maze);
		}
	}
}

bool Maze::containsUnvisited(MazeNode & node)
{
	for (int index = 0; index < node.getNumOfConnections(); index++)
	{
		if (node.getNode(index) != 0 && !(node.getNode(index)->isVisited()))
			return true;
	}

	return false;
}