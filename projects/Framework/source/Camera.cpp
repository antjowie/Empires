#include "Camera.h"

#include <glm/glm/ext/matrix_transform.hpp>
#include <glm/glm/ext/matrix_clip_space.hpp>

Camera::Camera():
	m_pos(0.f),
	m_target(glm::vec3(0.f,0.f,-1.f)),
	m_nearFar(glm::vec2(0.1f,100.f)),
	m_fov(45.f),
	m_aspectRatio(4.f/3.f)
{
}

void Camera::move(const glm::vec3 & pos)
{
	m_pos += pos;
}

void Camera::setPos(const glm::vec3 & pos)
{
	m_pos = pos;
}

void Camera::moveTarget(const glm::vec3 & pos)
{
	m_target += pos;
}

void Camera::setTarget(const glm::vec3 & pos)
{
	m_target = pos;
}

void Camera::setFOV(float degrees)
{
	assert(degrees > 0.f);
	m_fov = degrees;
}

void Camera::setAspectRatio(float aspectRatio)
{
	assert(aspectRatio > 0.f);
	m_aspectRatio = aspectRatio;
}

void Camera::setNearFar(const glm::vec2 &nearFar)
{
	assert(m_nearFar.x > 0.f && m_nearFar.y > m_nearFar.x);
	m_nearFar = nearFar;
}

const glm::vec3 & Camera::pos() const
{
	return m_pos;
}

const glm::vec3 & Camera::target() const
{
	return m_target;
}

float Camera::fov() const
{
	return m_fov;
}

float Camera::aspectRatio() const
{
	return m_aspectRatio;
}

const glm::vec2 &Camera::nearFar() const
{
	return m_nearFar;
}

glm::mat4 Camera::perspective() const
{
	return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearFar.x, m_nearFar.y);
}

glm::mat4 Camera::orthogonal() const
{
	return glm::ortho(-1.f,1.f,-1.f,1.f,m_nearFar.x,m_nearFar.y);
}

glm::mat4 Camera::view() const
{
	return glm::lookAt(m_pos, m_target, glm::vec3(0.f, 1.f, 0.f));
}


void Camera::handleInput(const Input &input, float elapsedTime)
{
}