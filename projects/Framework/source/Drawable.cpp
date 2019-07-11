#include "Drawable.h"

#undef countof
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/ext/matrix_transform.hpp>

#include <cstdio>

Drawable::Drawable(const DrawData & drawData):
	m_pos(0.f),
	m_rotation(0.f),
	m_scale(1.f),
	m_drawData(drawData),
	m_color(1)
{
}

Drawable::~Drawable()
{
}

void Drawable::move(const glm::vec3 & pos)
{
	m_pos += pos;
}

void Drawable::setPos(const glm::vec3 & pos)
{
	m_pos = pos;
}

void Drawable::rotate(const glm::vec3 & degrees)
{
	m_rotation += degrees;
}

void Drawable::setRotation(const glm::vec3 & degrees)
{
	m_rotation = degrees;
}

void Drawable::setScale(const glm::vec3 & scale)
{
	m_scale = scale;
}

void Drawable::setScale(float scale)
{
	setScale(glm::vec3(scale));
}

const glm::vec3 &Drawable::pos() const
{
	return m_pos;
}

const glm::vec3 & Drawable::rotation() const
{
	return m_rotation;
}

const glm::vec3 & Drawable::scale() const
{
	return m_scale;
}

void Drawable::draw(const Camera &camera) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_drawData.vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_drawData.ebo);

	glUseProgram(m_drawData.program);

	const glm::mat4 mvp = camera.perspective() * camera.view() * toWorld();
	
	glUniformMatrix4fv(glGetUniformLocation(m_drawData.program, "mvp"), 1, GL_FALSE, &mvp[0][0]);
	glUniform3fv(glGetUniformLocation(m_drawData.program, "color"), 1, &m_color[0]);

	glVertexAttribPointer(m_drawData.posLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(m_drawData.texLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(m_drawData.posLoc);
	glEnableVertexAttribArray(m_drawData.texLoc);

	glBindTexture(GL_TEXTURE_2D, m_drawData.textureID);
	//glUniform1i(m_drawData.samLoc, 0);

	//glDrawElements(GL_TRIANGLES, m_drawData.indicesCount, GL_UNSIGNED_SHORT, 0);
	glDrawArrays(GL_TRIANGLES, 0, m_drawData.indicesCount);
}

void Drawable::drawTransparent(const Camera & camera) const
{
}

glm::mat4 Drawable::toWorld() const
{
	glm::mat4 trans(1.f);
	trans = glm::translate(trans, pos());
	trans = glm::scale(trans, m_scale);
	trans = glm::rotate(trans, glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));
	trans = glm::rotate(trans, glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
	trans = glm::rotate(trans, glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
	return trans;
}
