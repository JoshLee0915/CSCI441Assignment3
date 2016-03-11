#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <exception>
#include <iostream>
#include "Maze3d.h"
#include "OpenGLShaderLoader.h"

using std::cout;

glm::vec2 Maze3d::getStartpos()
{
	glm::vec2 pos;

	int centerX = (maze[0].length() / 2);
	int centerY = (maze.size() / 2);

	pos.x = (centerX - (cubeSize*(float)startX))*wallScaler;
	pos.y = (centerY - (cubeSize*(float)startZ))*wallScaler;

	return pos;
}

Maze3d::Maze3d(int width, int height, int trapChance, bool failSafe)
{
	try
	{
		rawMaze = new Maze(width, height, trapChance, failSafe);
		wallProg = OpenGLShaderLoader::getInstance().createProgram("SquareVertexShader.glsl", "passThroughFrgtShader.glsl");
	}
	catch (exception& e)
	{
		cout << e.what();
		exit(1);
	}

	this->trapChance = trapChance;
	maze = createMazeVector(rawMaze->dispayMaze());
}

Maze3d::~Maze3d()
{
	delete rawMaze;
	OpenGLShaderLoader::getInstance().deleteProgram(wallProg);
}

void Maze3d::renderMaze(glm::mat4 view, glm::mat4 projection, GLfloat wallHeightScaler)
{ 
	vector<string> baseMaze = maze;
	
	// calc the start x and y corrds
	int xStart = (baseMaze[0].length() / 2);
	int yStart = (baseMaze.size() / 2);

	for (int y = 0; y < baseMaze.size(); y++)
	{
		for (int x = 0; x < baseMaze[y].length(); x++)
		{
			// check if a cube should be drawn
			if (baseMaze[y][x] == '#')
			{
				// calc the translateion
				GLfloat xTrans = (xStart - (cubeSize*x))*wallScaler;
				GLfloat zTrans = (yStart - (cubeSize*y))*wallScaler;

				// draw the wall
				drawWall(glm::translate(glm::mat4(1.0f), glm::vec3(xTrans, 0.0, zTrans)), 
						glm::scale(glm::mat4(1.0f), glm::vec3(wallScaler*0.5, wallHeightScaler, wallScaler*0.5)), view, projection);
			}
		}
	}
}

bool Maze3d::collision(GLfloat x, GLfloat y)
{
	// get the grid
	glm::vec2 grid = worldToMap(x, y);

	// check if in of bounds
	if (grid.x < 0 || grid.y < 0 || grid.y >= maze.size() || grid.x >= maze[grid.y].length())
		return false;

	// check for a wall
	if (maze[grid.y][grid.x] == '#')
		return true;

	return false;
}

bool Maze3d::checkForTrap(GLfloat x, GLfloat y)
{
	// get the grid
	glm::vec2 grid = worldToMap(x, y);

	// check if in of bounds
	if (grid.x < 0 || grid.y < 0 || grid.y >= maze.size() || grid.x >= maze[grid.y].length())
		return false;

	// check for a wall
	if (maze[grid.y][grid.x] == 'T')
	{
		// create a new maze with the same dimesions
		reinitMaze();
		return true;
	}

	return false;
}

string Maze3d::dispayMaze()
{
	string mzstr = rawMaze->dispayMaze();
	// hide the traps
	//for (int c = 0; c < mzstr.length(); c++)
	//{
	//	if (mzstr[c] == 'T')
	//		mzstr[c] = ' ';
	//}
	return mzstr;
}

glm::vec2 Maze3d::worldToMap(GLfloat x, GLfloat y)
{
	float gridX, gridY;
	// calc the start x and y corrds
	int xStart = (maze[0].length() / 2);
	int yStart = (maze.size() / 2);

	gridX = ((((x / wallScaler) - xStart) / -cubeSize)+0.5);
	gridY = ((((y / wallScaler) - yStart) / -cubeSize)+0.5);

	return glm::vec2(gridX, gridY);
}

void Maze3d::reinitMaze()
{
	int width = rawMaze->getMazeWidth();
	int height = rawMaze->getMazeHeight();
	delete rawMaze;
	rawMaze = new Maze(width, height, trapChance);
	maze = createMazeVector(rawMaze->dispayMaze());
}

void Maze3d::drawWall(glm::mat4 translate, glm::mat4 scale, glm::mat4 view, glm::mat4 projection)
{
	glUseProgram(wallProg);

	// get the uniforms
	GLuint transUni = glGetUniformLocation(wallProg, "trans");
	GLuint scaleUni = glGetUniformLocation(wallProg, "scale");
	GLuint viewUni = glGetUniformLocation(wallProg, "viewMatrix");
	GLuint projecUni = glGetUniformLocation(wallProg, "projection");

	// set the translation of the wall
	glUniformMatrix4fv(transUni, 1, GL_FALSE, glm::value_ptr(translate));
	glUniformMatrix4fv(scaleUni, 1, GL_FALSE, glm::value_ptr(scale));
	glUniformMatrix4fv(viewUni, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projecUni, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_QUADS, 0, 24);
}

vector<string> Maze3d::createMazeVector(string maze)
{
	string line = "";
	vector<string> mazeVec;

	for (int c = 0; c < maze.length(); c++)
	{
		if (maze[c] == '\n')
		{
			// append the line
			mazeVec.push_back(line);
			// clear the line
			line = "";
		}
		else
			line += maze[c];
	}

	// create two openings
	while (true)
	{
		int col = rand() % mazeVec[0].length();
		if (mazeVec[1][col] == ' ' && col >  mazeVec[0].length()/2)
		{
			mazeVec[0][col] = ' ';
			break;
		}
	}

	while (true)
	{
		int col = rand() % mazeVec[mazeVec.size()-1].length();
		if (mazeVec[mazeVec.size() - 2][col] == ' ' && col <  mazeVec[mazeVec.size() - 1].length() / 2)
		{
			startX = col;
			startZ = mazeVec.size() - 1;
			mazeVec[startZ][startX] = ' ';
			break;
		}
	}

	return mazeVec;
}
