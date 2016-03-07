#pragma once
#include <string>
#include <vector>
#include <map>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Vertex.h"

using std::string;
using std::vector;
using std::map;

struct Model
{
	unsigned int vao;
	vector<unsigned int> vbo;
};

class GameModels
{
private:
	map<string, Model> modelList;
public:
	GameModels();
	~GameModels();
	unsigned int getModel(string modelName);
	void createTriangleModel(string modelName);
	void deleteModel(string modelName);
};

