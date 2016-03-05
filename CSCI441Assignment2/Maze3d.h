#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <vector>
#include "Maze.h"

using std::vector;

class Maze3d : public Maze
{
public:
	Maze3d(int width, int height, bool failSafe = true);
	~Maze3d();
	void renderMaze(GLfloat camX, GLfloat camY, GLfloat camZ, GLfloat upx, GLfloat upy, GLfloat upz, GLfloat wallHeightScaler);
private:
	void drawWall();
	vector<string> createMazeVector(string maze);
};

