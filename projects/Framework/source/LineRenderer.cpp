#include "LineRenderer.h"
#include "Shader.h"

#include <GLES2/gl2.h>

LineRenderer::LineRenderer():
	m_vbo(0),
	m_maxVertices(0),
	m_program(0),
	m_posLoc(0),
	m_colLoc(0),
	m_mvpLoc(0)
{
}

bool LineRenderer::init(unsigned maxVertices, const float lineWidth)
{
	// If this happens, we have to increase buffer size
	// I'll see if that becomes necessary
	m_maxVertices = maxVertices;
	m_vertices.reserve(m_maxVertices);

	glLineWidth(lineWidth);

	// Create the program
	m_program = glCreateProgram();
	Shader shader;
	bool success = true;
	success = shader.addShader(GL_VERTEX_SHADER, "resources/shaders/line.vert");
	if (!success)
	{
		std::cout << "Failed to load line vertex shader\n";
		return false;
	}

	success = shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/line.frag");
	if (!success)
	{
		std::cout << "Failed to load line fragment shader\n";
		return false;
	}

	shader.linkProgram(m_program);
	if (!success)
	{
		std::cout << "Failed to link line shaders\n";
		return false;
	}

	// Get attribute locations
	m_posLoc = glGetAttribLocation(m_program, "aVerPos");
	m_colLoc = glGetAttribLocation(m_program, "aColPos");
	m_mvpLoc = glGetUniformLocation(m_program, "mvp");

	if (m_posLoc < 0 || m_colLoc < 0 || m_mvpLoc < 0)
	{
		std::cout << "Couldn't get the attribute positions from line shader\n";
		return false;
	}

	// Create the VBO
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, maxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void LineRenderer::addLine(const Line & line)
{
	Vertex a;
	a.pos = line.start;
	Vertex b;
	b.pos = line.end;

	a.color = b.color = line.color;

	m_vertices.push_back(a);
	m_vertices.push_back(b);
}

void LineRenderer::display(const Camera & camera)
{
	assert(m_vertices.size() <= m_maxVertices);

	glUseProgram(m_program);

	const glm::mat4 mvp = camera.perspective() * camera.view();
	glUniformMatrix4fv(m_mvpLoc, 1, GL_FALSE, &mvp[0][0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vertex), m_vertices.data());

	glVertexAttribPointer(m_posLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glVertexAttribPointer(m_colLoc, 3, GL_FLOAT, GL_TRUE,  6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

	glEnableVertexAttribArray(m_posLoc);
	glEnableVertexAttribArray(m_colLoc);

	glDrawArrays(GL_LINES, 0, m_vertices.size());
	m_vertices.clear();
}

Line::Line(const glm::vec3 & start, const glm::vec3 & end, const glm::vec3 & color):
	start(start),
	end(end),
	color(color)
{
}