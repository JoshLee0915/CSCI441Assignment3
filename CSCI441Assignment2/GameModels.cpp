#include "GameModels.h"



GameModels::GameModels()
{}


GameModels::~GameModels()
{
	for (map<string, Model>::iterator it = modelList.begin(); it != modelList.end(); it++)
	{
		glDeleteVertexArrays(1, &it->second.vao);
		glDeleteBuffers(it->second.vbo.size(), &it->second.vbo[0]);
		it->second.vbo.clear();
	}
	modelList.clear();
}

unsigned int GameModels::getModel(string modelName)
{
	return modelList[modelName].vao;
}

void GameModels::createTriangleModel(string modelName)
{
	unsigned int vbo;
	Model triangleModel;
	vector<Vertex> vertices;

	// create the vao
	glGenVertexArrays(1, &triangleModel.vao);
	glBindVertexArray(triangleModel.vao);

	// create triangle
	vertices.push_back(vec3(0.25, -0.25, 0.0));
	vertices.push_back(vec3(-0.25, -0.25, 0.0));
	vertices.push_back(vec3(0.25, 0.25, 0.0));

	// create the vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	triangleModel.vbo.push_back(vbo);
	modelList[modelName] = triangleModel;
}

void GameModels::deleteModel(string modelName)
{
	Model model = modelList[modelName];
	glDeleteVertexArrays(1, &model.vao);
	glDeleteBuffers(model.vbo.size(), &model.vbo[0]);
	model.vbo.clear();
	modelList.erase(modelName);
}
