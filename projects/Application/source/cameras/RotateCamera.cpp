#include "cameras/RotateCamera.h"
#include <glm/glm/gtc/constants.hpp>
#include <glm/glm/trigonometric.hpp>

RotateCamera::RotateCamera(const glm::vec3 &target, float radius, float rotationTime, float pitch, bool bounce) :
	m_yaw(0),
	m_pitch(glm::radians(pitch)),
	m_radius(radius),
	m_rotationTime(1.f / rotationTime),
	m_bounce(bounce),
	m_target(target)
{
	setTarget(target);
}

void RotateCamera::handleInput(const Input &input, float elapsedTime)
{
	m_yaw += glm::two_pi<float>() * m_rotationTime * elapsedTime;
	if (m_yaw > glm::two_pi<float>() * 100.f)
		m_yaw = 0.f;

	glm::vec3 pos = glm::vec3(
		glm::sin(m_yaw) * m_radius, 0,
		glm::cos(m_yaw) * m_radius);

	if (m_bounce)
	{
		pos.y = glm::sin(m_yaw) * m_pitch;
	}

	else
	{
		pos.y = glm::sin(m_pitch);
	}

	setPos(pos + m_target);
}