#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <vector>
#include "Maze.h"

using std::vector;

class Maze3d
{
private:
	GLuint wallProg;
	vector<string> maze;
	Maze* rawMaze;
	int startX;
	int startZ;
	int trapChance;
	const GLfloat cubeSize = 1.0;
	const GLfloat wallScaler = 0.60;
	const GLfloat collisionBuffer = 0.05;
public:
	glm::vec2 getStartpos();
	Maze3d(int width, int height, int trapChance, bool failSafe = true);
	~Maze3d();
	void renderMaze(glm::mat4 view, glm::mat4 projection, GLfloat wallHeightScaler);
	bool collision(GLfloat x, GLfloat y);
	bool checkForTrap(GLfloat x, GLfloat y);
	string dispayMaze();
private:
	void drawWall(glm::mat4 translate, glm::mat4 scale, glm::mat4 view, glm::mat4 projection);
	vector<string> createMazeVector(string maze);
	glm::vec2 worldToMap(GLfloat x, GLfloat y);
	void reinitMaze();
};

