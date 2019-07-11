#pragma once

#include "Input.h"
#undef countof
#include <glm/glm/mat4x4.hpp>

class Camera
{
private:
	glm::vec3 m_pos;
	glm::vec3 m_target;
	glm::vec2 m_nearFar;

	float m_fov;
	float m_aspectRatio;
	
public:
	Camera();

	virtual void handleInput(const Input &input, float elapsedTime);

	void move(const glm::vec3 &pos);
	void setPos(const glm::vec3 &pos);
	void moveTarget(const glm::vec3 &pos);
	void setTarget(const glm::vec3 &pos);
	void setFOV(float degrees);
	void setAspectRatio(float aspectRatio);
	void setNearFar(const glm::vec2 &nearFar);

	const glm::vec3 &pos() const;
	const glm::vec3 &target() const;
	float fov() const; // In degrees
	float aspectRatio() const;
	const glm::vec2 &nearFar() const;

	glm::mat4 perspective() const;
	glm::mat4 orthogonal() const;
	glm::mat4 view() const;
};