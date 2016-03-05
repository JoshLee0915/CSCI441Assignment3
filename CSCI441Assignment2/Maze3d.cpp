#include "Maze3d.h"

Maze3d::Maze3d(int width, int height, bool failSafe) : Maze(width, height, failSafe)
{}

Maze3d::~Maze3d()
{}

void Maze3d::renderMaze(GLfloat camX, GLfloat camY, GLfloat camZ, GLfloat upx, GLfloat upy, GLfloat upz, GLfloat wallHeightScaler)
{
	const GLfloat wallScaler = 0.5;
	vector<string> baseMaze = createMazeVector(dispayMaze());
	
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
				GLfloat xTrans = xStart - (1.0*x);
				GLfloat zTrans = yStart - (1.0*y);

				// begin draw
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

				// calc and preform transform/scale and camera positioning
				gluLookAt(camX, camY, camZ, 0, 0, 0, upx, upy, upz);
				glScalef(wallScaler, wallHeightScaler, wallScaler);
				glTranslatef(xTrans, 0, zTrans);

				// draw the wall
				drawWall();

				//glutSwapBuffers();
			}
		}
	}
}

void Maze3d::drawWall()
{
	glBegin(GL_QUADS);                
					
	// Green
	glColor3f(0, 1, 0);     
	glVertex3f(0.5, 0.1, -0.5);
	glVertex3f(-0.5, 0.1, -0.5);
	glVertex3f(-0.5, 0.1, 0.5);
	glVertex3f(0.5, 0.1, 0.5);

	// Orange
	glColor3f(1, 0.5, 0);    
	glVertex3f(0.5, 0, 0.5);
	glVertex3f(-0.5, 0, 0.5);
	glVertex3f(-0.5, 0, -0.5);
	glVertex3f(0.5, 0, -0.5);

	// Red
	glColor3f(1, 0, 0);     
	glVertex3f(0.5, 0.1, 0.5);
	glVertex3f(-0.5, 0.1, 0.5);
	glVertex3f(-0.5, 0, 0.5);
	glVertex3f(0.5, 0, 0.5);

	// Yellow
	glColor3f(1, 1, 0);     
	glVertex3f(0.5, 0, -0.5);
	glVertex3f(-0.5, 0, -0.5);
	glVertex3f(-0.5, 0.1, -0.5);
	glVertex3f(0.5, 0.1, -0.5);

	// Blue
	glColor3f(0, 0, 1);     
	glVertex3f(-0.5, 0.1, 0.5);
	glVertex3f(-0.5, 0.1, -0.5);
	glVertex3f(-0.5, 0, -0.5);
	glVertex3f(-0.5, 0, 0.5);

	// Magenta
	glColor3f(1, 0, 1);    
	glVertex3f(0.5, 0.1, -0.5);
	glVertex3f(0.5, 0.1, 0.5);
	glVertex3f(0.5, 0, 0.5);
	glVertex3f(0.5, 0, -0.5);
	glEnd();  
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

	return mazeVec;
}
