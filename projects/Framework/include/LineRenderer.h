#pragma once
#include "Camera.h"

#include <vector>
#include <glm/glm/vec3.hpp>

struct Line
{
public:
	glm::vec3 start;
	glm::vec3 end;
	glm::vec3 color;

	Line(const glm::vec3 &start, const glm::vec3 &end, const glm::vec3 &color);
};

class LineRenderer
{
private:
	struct Vertex
	{
	public:
		glm::vec3 pos;
		glm::vec3 color;
	};

	std::vector<Vertex> m_vertices;

	unsigned m_vbo;
	unsigned m_maxVertices;
	unsigned m_program;

	int m_posLoc;
	int m_colLoc;
	int m_mvpLoc;

public:
	LineRenderer();

	bool init(unsigned maxVertices, const float lineWidth = 1.f);

	void addLine(const Line &line);
	void display(const Camera &camera);
};