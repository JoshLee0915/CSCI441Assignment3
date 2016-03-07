#include <iostream>
#include <exception>
#include "OpenGLShaderLoader.h"
#include "GameModels.h"

#define WIDTH 800
#define HEIGHT 800

using std::cout;
using std::exception;

GLuint progId;
GameModels* gameModles;

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(gameModles->getModel("triangle1"));
	glUseProgram(progId);

	// square test
	/*glDrawArrays(GL_QUADS, 0, 4);*/

	// tutorial draw
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}

void close()
{
	cout << "GLUT:\t Finished" << std::endl;
	glutLeaveMainLoop();
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	// create the model
	gameModles = new GameModels();
	gameModles->createTriangleModel("triangle1");

	glViewport(0, 0, WIDTH, HEIGHT);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
								  // Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, ((GLfloat)WIDTH / (GLfloat)HEIGHT), 0.1f, 1000.0f);

	try
	{
		// load the shaders

		// square test
		/*progId = OpenGLShaderLoader::getInstance().createProgram("testVrtxShader.glsl", "testFrgtShader.glsl");*/

		// tutorial
		progId = OpenGLShaderLoader::getInstance().createProgram("testTriVertexShader.glsl", "testFrgtShader.glsl");
	}
	catch (exception& e)
	{
		cout << e.what();
		exit(1);
	}
	glPolygonMode(GL_FRONT, GL_FILL);
}

int main(int argc, char* args[])
{
	// create the window
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Opengl Maze");
	glewInit();

	init();

	glutDisplayFunc(renderScene);
	glutCloseFunc(close);

	glutMainLoop();

	// clean up
	delete gameModles;
	OpenGLShaderLoader::getInstance().deleteProgram(progId);
	return 0;
}