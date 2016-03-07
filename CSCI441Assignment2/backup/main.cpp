#include <iostream>
#include <string>
#include <sstream>
#include "Maze3d.h"

#define WIDTH 800
#define HEIGHT 800

// The ending transform factor for camera pos
#define Z_TRANS_FACT 1.25
#define Y_TRANS_FACT 0.5

// Max wall scaling factor
#define MAX_WALL_SCALE 10

// delay const for 60fps
#define FPS 60
#define FPS_DELAY (1/FPS)*1000

// animation Time To Target (TTT)
#define TRANS_TTT 3			// transform should finish in 2 seconds
#define WALL_SCALE_TTT 2	// wall scale should finish in 3 seconds

using std::cin;
using std::cout;
using std::stringstream;

void init();
void renderScene();
void renderTimer(int val);

Maze3d* maze;
GLfloat zoom;

// enum for the animation modes
enum AnimMode
{
	initial,
	cameraTilt,
	raiseWalls,
	noop
};

int main(int argc, char* args[])
{
	int mazeWidth = 0;
	int mazeHeight = 0;
	string height;
	string width;

	// prompt the user for input
	cout << "Please enter the mazes parmiters. Please note for best results keep maze under 2500 cells - (50x50)\nMaze Width: ";
	cin >> width;
	cout << "Maze Height: ";
	cin >> height;

	cout << "Generating " + width + "x" + height + " Maze\nPlease Wait...\n";
	// create the maze
	try
	{
		stringstream widthSS(width);
		stringstream heightSS(height);

		widthSS >> mazeWidth;
		heightSS >> mazeHeight;

		// check if input is valid
		if (mazeWidth < 2 && mazeHeight < 2)
		{
			cout << "[ERR]Invalid min size for maze\n";
			return 1;
		}
		// set the zoom factor based on the largest part of the maze
		zoom = mazeWidth > mazeHeight ? mazeWidth * 2 : mazeHeight * 2;
		maze = new Maze3d(mazeWidth, mazeHeight);

		// display maze Map
		cout << "MAP:\n" << maze->dispayMaze() << "\n";
	}
	catch (std::exception& msg)
	{
		cout << msg.what();
		return 1;
	}

	// create the window
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Opengl Maze");
	glewInit();

	init();

	glutDisplayFunc(renderScene);
	glutTimerFunc(FPS_DELAY, renderTimer, 0);
	glutMainLoop();

	delete maze;
	return 0;
}

void renderScene()
{
	// keep track of what transformation to do
	// 0 - start state 1 - camera tilt 2 - extrude walls 3 - noop
	static AnimMode mode = AnimMode::initial;

	// values that will be chaned on every call to renderScene till mode 3
	static double cameraYTrans = 1;
	static double cameraZTrans = 0;
	static double wallScalingFactor = 0;

	// caclulate the delta for the transformations and scaling
	static const double Y_TRANS_DELETA = (cameraYTrans - Y_TRANS_FACT) / (FPS*TRANS_TTT);
	static const double Z_TRANS_DELETA = (Z_TRANS_FACT - cameraZTrans) / (FPS*TRANS_TTT);
	static const double WALL_SCALE_DELETA = (double)MAX_WALL_SCALE / (FPS*WALL_SCALE_TTT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (mode)
	{
	// init mode
	case AnimMode::initial:
		mode = AnimMode::cameraTilt;	// move to camera tilt mode
		break;
	case AnimMode::cameraTilt:
		cameraYTrans -= Y_TRANS_DELETA;
		cameraZTrans += Z_TRANS_DELETA;

		// check if the camera is in position
		if(cameraYTrans <= Y_TRANS_FACT)
			mode = AnimMode::raiseWalls;	// move to raise walls mode
		break;
	case AnimMode::raiseWalls:
		wallScalingFactor += WALL_SCALE_DELETA;

		// check if the walls are at the max height
		if(wallScalingFactor >= MAX_WALL_SCALE)
			mode = AnimMode::noop;			// move to dead mode
		break;
	default:
		/* noop */
		break;
	}

	// renderMaze
	maze->renderMaze(0, zoom*cameraYTrans, -(zoom*cameraZTrans), 0, 0, 1, wallScalingFactor);

	glutSwapBuffers();
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, WIDTH, HEIGHT);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
								  // Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, ((GLfloat)WIDTH / (GLfloat)HEIGHT), 0.1f, 1000.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void renderTimer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(FPS_DELAY, renderTimer, 0);
}