#pragma once
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

using std::string;
using std::ifstream;
using std::exception;

class OpenGLShaderLoader
{
public:
	OpenGLShaderLoader();
	~OpenGLShaderLoader();

	GLuint createProgram(string vertexShaderFile, string fragmentShaderFile);
private:
	string loadShader(string fileName);
	GLuint createShader(GLenum shaderType, string src, string shaderName);
};

