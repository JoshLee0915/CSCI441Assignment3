#include <iostream>
#include <string>
#include <sstream>
#include "Maze3d.h"
#include <glm/gtc/matrix_transform.hpp>

// window size
#define WIDTH 800
#define HEIGHT 800

// The ending transform factor for camera pos
#define Z_TRANS_FACT 1.25
#define Y_TRANS_FACT 0.2

// the camera should always be at this level
#define Y_CAM_POS 0.3

// Max wall scaling factor
#define MAX_WALL_SCALE 5

// delay const for 60fps
#define FPS 60
#define FPS_DELAY (1/FPS)*1000

// animation Time To Target (TTT)
#define TRANS_TTT 3			// transform should finish in 3 seconds
#define WALL_SCALE_TTT 2	// wall scale should finish in 2 seconds

using std::cin;
using std::cout;
using std::stringstream;

using glm::vec3;
using glm::mat4;
using glm::lookAt;
using glm::perspective;

void init();
void renderScene();
void renderTimer(int val);
void keyPress(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mousePosTracker(int x, int y);
void play(mat4& view);
vec3 calcCameraFront(float x, float y);
bool intro(mat4& view, double& wallScale);
void resetWalls(double& wallScale);

Maze3d* maze;
GLfloat zoom;
mat4 proj;

// key press buffer
bool keyStates[256];

// trap triger
bool triggeredTrap = false;

// front of the camera
vec3 camFront;

// enum for the animation modes
enum AnimMode
{
	initial,
	cameraTilt,
	raiseWalls,
	moveToStart,
	noop
};

int main(int argc, char* args[])
{
	int mazeWidth = 0;
	int mazeHeight = 0;
	int mazeDifficulty = 0;
	string height;
	string width;
	string difficulty;

	// prompt the user for input
	cout << "Please enter the mazes parmiters. Please note for best results keep maze under 2500 cells - (50x50)\nMaze Width: ";
	cin >> width;
	cout << "Maze Height: ";
	cin >> height;
	cout << "Please chose a difficulty:\n1. Grade School Home Work (0% trap chance)\n2. Hedge Maze(10% trap chance)\n3. Corn Maze(25% trap chance)\n4. Brain Teaser(40% trap chance)\n5. The Living Maze(65% trap chance)\n6. Deadalus's Labyrinth(80% trap chance)\n> ";
	cin >> difficulty;

	cout << "Generating " + width + "x" + height + " Maze with a difficulty of: " + difficulty + "\nPlease Wait...\n";

	// create the window
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Opengl Maze");
	glewInit();

	// create the maze
	try
	{
		stringstream widthSS(width);
		stringstream heightSS(height);
		stringstream difficultySS(difficulty);

		widthSS >> mazeWidth;
		heightSS >> mazeHeight;
		difficultySS >> mazeDifficulty;

		switch (mazeDifficulty)
		{
		case 1:
			mazeDifficulty = 0;
			break;
		case 2:
			mazeDifficulty = 10;
			break;
		case 3:
			mazeDifficulty = 25;
			break;
		case 4:
			mazeDifficulty = 40;
			break;
		case 5:
			mazeDifficulty = 65;
			break;
		case 6:
			mazeDifficulty = 80;
			break;
		default:
			mazeDifficulty = 0;
			break;
		}

		// check if input is valid
		if (mazeWidth < 2 && mazeHeight < 2)
		{
			cout << "[ERR]Invalid min size for maze\n";
			return 1;
		}
		// set the zoom factor based on the largest part of the maze
		zoom = mazeWidth > mazeHeight ? mazeWidth * 2 : mazeHeight * 2;
		maze = new Maze3d(mazeWidth, mazeHeight, mazeDifficulty);

		// display maze Map
		cout << "MAP:\n" << maze->dispayMaze() << "\n";
	}
	catch (std::exception& msg)
	{
		cout << msg.what();
		return 1;
	}

	init();

	glutDisplayFunc(renderScene);
	glutTimerFunc(FPS_DELAY, renderTimer, 0);
	glutPassiveMotionFunc(mousePosTracker);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutMainLoop();

	delete maze;
	return 0;
}

void renderScene()
{
	static bool reset = false;
	static mat4 view;
	static double wallScalingFactor = 0;

	// play the intro then the game
	if (!intro(view, wallScalingFactor))
	{
		if (triggeredTrap)
		{
			if (reset)
			{
				// display maze Map
				cout << "MAP:\n" << maze->dispayMaze() << "\n";
				wallScalingFactor = 0;
			}

			resetWalls(wallScalingFactor);
			reset = false;
		}
		else
		{
			play(view);
			reset = true;
		}
	}

	maze->renderMaze(view, proj, wallScalingFactor);

	glutSwapBuffers();
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, WIDTH, HEIGHT);

	proj = perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

	maze->getStartpos();
}

void renderTimer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(FPS_DELAY, renderTimer, 0);
}

void keyPress(unsigned char key, int x, int y)
{
	// set key as pressed
	keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y)
{
	// set key as not pressed
	keyStates[key] = false;
}

void mousePosTracker(int x, int y)
{
	const static double MOUSE_SENSITVITY = 0.4;
	static int lastX = x;
	static int lastY = y;

	// calc the Delta
	float xDelta = (float)(x - lastX)*MOUSE_SENSITVITY;
	float yDelta = (float)(y - lastY)*MOUSE_SENSITVITY;

	// store the current values to calc the offset next time
	lastX = x;
	lastY = y;

	// calc the front
	camFront = calcCameraFront(xDelta, yDelta);
}

void play(mat4& view)
{
	static bool start = true;
	static const float speed = 0.05;
	static vec3 cameraPos = vec3(maze->getStartpos().x, Y_CAM_POS, maze->getStartpos().y);
	vec3 backUpPos = cameraPos;
	vec3 cameraFront = camFront;

	// check if the payer triggered a trap
	if ((triggeredTrap = maze->checkForTrap(cameraPos.x, cameraPos.z)))
	{
		cout << "The maze seems to be shifting!\n\n";
		while (maze->checkForTrap(cameraPos.x, cameraPos.z));	// block till a proper maze is created
		return;
	}

	// move the camera
	if (keyStates['w'])
	{
		cameraPos += speed*cameraFront;
		start = false;
	}
	if (keyStates['s'])
	{
		cameraPos -= speed*cameraFront;
		start = false;
	}
	if (keyStates['a'])
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, vec3(0, 1, 0)))*speed;
		start = false;
	}
	if (keyStates['d'])
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, vec3(0, 1, 0)))*speed;
		start = false;
	}

	// check for collision
	if (maze->collision(cameraPos.x, backUpPos.z))
		cameraPos.x = backUpPos.x;		// do not allow the transformation on the x
	if (maze->collision(backUpPos.x, cameraPos.z))
		cameraPos.z = backUpPos.z;		// do not allow the transformation on the z

	// set the Y component to the fixed value
	cameraPos.y = Y_CAM_POS;

	// check if the player is at the start or not
	vec3 cameraPos2 = start ? vec3(0, 0, 0) : cameraPos;

	view = lookAt(cameraPos, cameraPos2+cameraFront, vec3(0, 1, 0));	// y is always our up vec

}

vec3 calcCameraFront(float x, float y)
{
	vec3 front;
	static float yaw = 0;
	static float pitch = 0;

	// max pitch
	const static GLfloat MAX_PITCH = 89;

	// calc the pitch and yaw of the camera
	yaw += x;
	pitch += y;

	// check our pitch limits
	pitch = pitch > MAX_PITCH ? MAX_PITCH : pitch;
	pitch = pitch < (-1 * MAX_PITCH) ? -1 * MAX_PITCH : pitch;

	// calc the vector
	front.x = cos(glm::radians(yaw));
	front.y = -sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw));

	//cout << "yaw: " << front.x << std::endl << "pitch: " << front.z << std::endl;

	return glm::normalize(front);
}

bool intro(mat4& view, double& wallScale)
{
	static vec3 camPos;

	// keep track of what transformation to do
	// 0 - start state 1 - camera tilt 2 - extrude walls 3 - noop
	static AnimMode mode = AnimMode::initial;

	// values that will be chaned on every call to renderScene till mode 3
	static double cameraYTrans = 1;
	static double cameraZTrans = 0;

	// caclulate the delta for the transformations and scaling
	static const double WALL_SCALE_DELTA = (double)MAX_WALL_SCALE / (FPS*WALL_SCALE_TTT);

	static double xDelta;
	static double yDelta;
	static double zDelta;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (mode)
	{
		// init mode
	case AnimMode::initial:
		mode = AnimMode::cameraTilt;	// move to camera tilt mode
										// set the deltas
		yDelta = (1 - Y_TRANS_FACT) / (FPS*TRANS_TTT);
		zDelta = Z_TRANS_FACT / (FPS*TRANS_TTT);
		break;
	case AnimMode::cameraTilt:
		cameraYTrans -= yDelta;
		cameraZTrans += zDelta;

		camPos = vec3(0.0, (zoom*cameraYTrans), -(zoom*cameraZTrans));

		// check if the camera is in position
		if (cameraYTrans <= Y_TRANS_FACT)
			mode = AnimMode::raiseWalls;	// move to raise walls mode
		break;
	case AnimMode::raiseWalls:
		wallScale += WALL_SCALE_DELTA;

		// check if the walls are at the max height
		if (wallScale >= MAX_WALL_SCALE)
		{
			xDelta = maze->getStartpos().x / (FPS*TRANS_TTT);
			yDelta = (camPos.y - Y_CAM_POS) / (FPS*TRANS_TTT);
			zDelta = (maze->getStartpos().y - camPos.z) / (FPS*TRANS_TTT);
			mode = AnimMode::moveToStart;
		}
		break;
	case AnimMode::moveToStart:
		camPos = vec3(camPos.x + xDelta, camPos.y - yDelta, camPos.z + zDelta);

		if (camPos.x >= maze->getStartpos().x)
			xDelta = 0;
		if (camPos.y <= Y_CAM_POS)
			yDelta = 0;
		if (camPos.z >= maze->getStartpos().y)
			zDelta = 0;

		if (xDelta == 0 && yDelta == 0 && zDelta == 0)
			mode = AnimMode::noop;
		break;
	default:
		return false;
		break;
	}

	// renderMaze
	view = lookAt(camPos, vec3(0.0, 0.0, 0.0), vec3(0, 1, 0));

	return true;
}

void resetWalls(double& wallScale)
{
	// caclulate the delta for the transformations and scaling
	static const double WALL_SCALE_DELTA = (double)MAX_WALL_SCALE / (FPS*WALL_SCALE_TTT);

	if (wallScale >= MAX_WALL_SCALE)
	{
		triggeredTrap = false;	// finshed
		return;
	}
	
	wallScale += WALL_SCALE_DELTA;
}