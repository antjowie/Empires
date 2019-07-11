#include "drawables/HUDs/HUD.h"
#include "hitboxes/HUDHitbox.h"
#include "glm/glm/gtc/matrix_transform.hpp"

HUD::HUD(const DrawData & drawData, const bool isClickable):
	Drawable(drawData),
	Clickable(new HUDHitbox(*this),isClickable),
	m_color(1)
{
}

void HUD::setColor(const glm::vec4 & color)
{
	m_color = color;
}

void HUD::update(const float elapsedTime)
{
}

void HUD::draw(const Camera &camera) const
{
	assert(false && "This should not be called, observe call stack");
}

void HUD::drawTransparent(const Camera & camera) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_drawData.vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_drawData.ebo);

	glUseProgram(m_drawData.program);

	// Can't get this to work for some reason so just manually multiply the transformations
	//glm::mat4 objToWorld(1);
	//glm::translate(objToWorld, glm::vec3(pos().x, pos().y, -0.1f));
	//glm::scale(objToWorld, scale());

	const glm::mat4 translation = glm::translate(glm::mat4(1), pos());
	const glm::mat4 scaled = glm::scale(glm::mat4(1), scale());
	const glm::mat4 mvp = camera.orthogonal() * translation * scaled * glm::mat4(1);

	glUniformMatrix4fv(glGetUniformLocation(m_drawData.program, "mvp"), 1, GL_FALSE, &mvp[0][0]);
	glUniform4fv(glGetUniformLocation(m_drawData.program, "color"), 1, &m_color[0]);

	glVertexAttribPointer(m_drawData.posLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(m_drawData.posLoc);

	glDrawArrays(GL_TRIANGLES, 0, m_drawData.indicesCount);
}