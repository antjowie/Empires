#include "PlanetRenderer.h"
#include "Shader.h"

PlanetRenderer::PlanetRenderer():
	m_vbo(0),
	m_program(0),
	m_vertices(0),
	m_isInitialized(false)
{
}

void PlanetRenderer::fillPlanetsVBO(const std::vector<std::unique_ptr<Planet>>& planets)
{
	// Convert planets to points
	std::vector<Vertex> vertices;
	for (const std::unique_ptr<Planet> &planet : planets)
	{
		Vertex vertex;
		vertex.pos = planet->pos();
		vertex.color = planet->color();
		vertex.scale = planet->scale().x * 1200.f;

		vertices.push_back(vertex);
	}

	if (!m_isInitialized)
	{
		m_program = glCreateProgram();
		Shader point;
		point.addShader(GL_VERTEX_SHADER, "resources/shaders/planet.vert");
		point.addShader(GL_FRAGMENT_SHADER, "resources/shaders/planet.frag");
		point.linkProgram(m_program);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	}
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_vertices = vertices.size();
	m_isInitialized = true;
}

void PlanetRenderer::display(const Camera &camera) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glUseProgram(m_program);
	
	const glm::mat4 mvp = camera.perspective() * camera.view();
	glUniformMatrix4fv(glGetUniformLocation(m_program, "mvp"), 1, GL_FALSE, &mvp[0][0]);

	const GLint verPos = glGetAttribLocation(m_program, "aVerPos");
	const GLint colPos = glGetAttribLocation(m_program, "aColPos");
	const GLint scale  = glGetAttribLocation(m_program, "aScale");

	glVertexAttribPointer(verPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colPos, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)(3* sizeof(GLfloat)));
	glVertexAttribPointer(scale, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),  (GLvoid*)(6* sizeof(GLfloat)));
	glEnable(verPos);
	glEnable(colPos);
	glEnable(scale);

	glDrawArrays(GL_POINTS, 0, m_vertices);
}