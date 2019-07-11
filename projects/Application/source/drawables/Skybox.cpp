#include "drawables/Skybox.h"
#include <glm/glm/ext/matrix_transform.hpp>

Skybox::Skybox(const DrawData & drawdata):
	Drawable(drawdata),
	m_color(0)
{
}

void Skybox::setColor(const glm::vec3 &color)
{
	m_color = glm::vec4(color,1);
}

void Skybox::draw(const Camera & camera) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_drawData.vbo);
	
	glUseProgram(m_drawData.program);

	const glm::mat4 mvp = camera.orthogonal() * glm::translate(glm::mat4(1),glm::vec3(0,0,-camera.nearFar().y));
	glUniformMatrix4fv(glGetUniformLocation(m_drawData.program, "mvp"), 1, GL_FALSE, &mvp[0][0]);
	glUniform4fv(glGetUniformLocation(m_drawData.program, "color"), 1, &m_color[0]);

	glVertexAttribPointer(m_drawData.posLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(m_drawData.posLoc);

	//glDrawElements(GL_TRIANGLES, m_drawData.indicesCount, GL_UNSIGNED_SHORT, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_drawData.indicesCount);
}