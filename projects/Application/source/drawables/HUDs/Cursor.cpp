#include "drawables/HUDs/Cursor.h"
#include <glm/glm/ext/matrix_transform.hpp>

Cursor::Cursor(const DrawData & drawData):
	HUD(drawData)
{
}

void Cursor::drawTransparent(const Camera & camera) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_drawData.vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_drawData.ebo);

	glUseProgram(m_drawData.program);

	// Can't get this to work for some reason so just manually multiply the transformations
	//glm::mat4 objToWorld(1);
	//glm::translate(objToWorld, glm::vec3(pos().x, pos().y, -0.1f));
	//glm::scale(objToWorld, scale());

	glm::vec2 scaleOffset(scale());
	scaleOffset.y *= -1.f;

	const glm::mat4 translation = glm::translate(glm::mat4(1), pos() + glm::vec3(scaleOffset,0));
	const glm::mat4 scaled = glm::scale(glm::mat4(1), scale());
	const glm::mat4 mvp = camera.orthogonal() * translation * scaled * glm::mat4(1);

	glUniformMatrix4fv(glGetUniformLocation(m_drawData.program, "mvp"), 1, GL_FALSE, &mvp[0][0]);
	
	glVertexAttribPointer(m_drawData.posLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(m_drawData.posLoc);

	glVertexAttribPointer(m_drawData.texLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(m_drawData.texLoc);

	glBindTexture(GL_TEXTURE_2D, m_drawData.textureID);

	glDrawArrays(GL_TRIANGLES, 0, m_drawData.indicesCount);
}
