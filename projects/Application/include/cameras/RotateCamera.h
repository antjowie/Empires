#pragma once
#include "Camera.h"

class RotateCamera : public Camera
{
private:
	float m_yaw;
	float m_pitch;
	float m_radius;

	float m_rotationTime;
	bool m_bounce;

	const glm::vec3 m_target;

public:
	RotateCamera(const glm::vec3 &target, float radius, float rotationTime, float pitch = 0, bool bounce = false);

	// Use this as an update which is a bad thing but its temporarily
	virtual void handleInput(const Input &input, float elapsedTime) override final;
};