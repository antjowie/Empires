#include "Hitbox.h"

Ray::Ray(const glm::vec3 & origin, const glm::vec3 & direction, const glm::vec2 &mousePos) :
	m_origin(origin),
	m_direction(glm::normalize(direction)),
	m_mousePos(mousePos)
{
}

const glm::vec3 & Ray::origin() const
{
	return m_origin;
}

const glm::vec3 & Ray::direction() const
{
	return m_direction;
}

const glm::vec2 & Ray::mousePos() const
{
	return m_mousePos;
}

Hitbox::Hitbox(const Drawable & drawable) :
	m_drawable(drawable)
{
}

bool Hitbox::contains(const glm::vec3 & point) const
{
	return false;
}
