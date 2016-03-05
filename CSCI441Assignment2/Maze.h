#pragma once
#include <string>
#include <stdlib.h>     
#include <time.h>  

#include "MazeNode.h"

using std::string;

class Maze
{
protected:
	int width, height;
	MazeNode* root;

public:
	int getMazeWidth() const;
	int getMazeHeight() const;

	Maze(int width, int height, bool failSafe = true);
	virtual ~Maze();

	string dispayMaze();

private:
	void buildMaze();
	void rndDpthFrstGen(MazeNode& map, MazeNode& maze);
	void destroyBaseMap(MazeNode* node, MazeNode* start);
	void destroyMaze(MazeNode* node);
	void clearVisited(MazeNode* node);
	void buildCell(MazeNode& node, string* maze);
	bool containsUnvisited(MazeNode& node);
};

