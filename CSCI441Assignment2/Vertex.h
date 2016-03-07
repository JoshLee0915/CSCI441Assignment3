#pragma once
#include <glm/glm.hpp>

using glm::vec3;

struct Vertex
{
	vec3 pos;

	Vertex(const vec3& position)
	{
		pos = position;
	}
};