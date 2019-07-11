#pragma once
#include "drawables/Planet.h"

class PlanetRenderer
{
private:
	GLuint m_vbo;
	GLuint m_program;
	unsigned m_vertices;

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		float scale;
	};

	bool m_isInitialized;

public:
	PlanetRenderer();

	void fillPlanetsVBO(const std::vector<std::unique_ptr<Planet>> &planets);

	void display(const Camera &camera) const;
};