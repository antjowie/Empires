#include "cameras/FreelookCamera.h"

#undef countof
#include <glm/glm/ext/matrix_transform.hpp>

FreelookCamera::FreelookCamera(float sensitivity, float speed):
	m_mousePos(0.f),
	m_direction(glm::vec3(0.f,0.f,-1.f)),
	m_yaw(0),
	m_pitch(0),
	m_sensitivity(sensitivity),
	m_speed(speed)
{
	setTarget(m_direction);
}

void FreelookCamera::setSensitivity(float sensitivity)
{
	m_sensitivity = sensitivity;
}

float FreelookCamera::sensitivity() const
{
	return m_sensitivity;
}

void FreelookCamera::handleInput(const Input &input, float elapsedTime)
{
	// Move direction
	glm::vec3 movement(0);

	if (input.Keys[KEY_W])
		movement += ( m_direction * m_speed * elapsedTime);
	if (input.Keys[KEY_S])
		movement += (-m_direction * m_speed * elapsedTime);

	// Strafe
	const glm::vec3 right = glm::cross(m_direction, glm::vec3(0, 1, 0));
	if (input.Keys[KEY_A])
		movement += (-right * m_speed * elapsedTime);
	if (input.Keys[KEY_D])
		movement += ( right * m_speed * elapsedTime);
	
	// Up down
	if (input.Keys[KEY_SPACE])
		movement += ( glm::vec3(0,1,0) * m_speed * elapsedTime);
	if (input.Keys[KEY_LEFTSHIFT])
		movement += (-glm::vec3(0,1,0) * m_speed * elapsedTime);

	if (input.Keys[KEY_LEFTCTRL])
		movement *= 30.f;

	move(movement);

	// Look direction
	if (input.Keys[KEY_UP])
		m_pitch += m_sensitivity * elapsedTime;
	if (input.Keys[KEY_DOWN])
		m_pitch -= m_sensitivity * elapsedTime;
	if (input.Keys[KEY_LEFT])
		m_yaw -= m_sensitivity * elapsedTime;
	if (input.Keys[KEY_RIGHT])
		m_yaw += m_sensitivity * elapsedTime;
	m_pitch = glm::clamp(m_pitch, -89.f, 89.f);

	// Can't get mouse to work because it keeps going ot of screen
	//const glm::vec2 oldMousePos = m_mousePos;
	//m_mousePos = input.GetMousePos();
	//const glm::vec2 deltaMouse = m_mousePos - oldMousePos;
	//m_yaw += deltaMouse.x * m_sensitivity;
	//m_pitch += deltaMouse.y * m_sensitivity;
	//glm::clamp(m_pitch, -89.f, 89.f);

	m_direction = glm::normalize(glm::vec3(
		glm::cos(glm::radians(m_pitch)) * glm::sin(glm::radians(m_yaw)),
		glm::sin(glm::radians(m_pitch)),
		-glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw))));

	//m_direction = glm::normalize(glm::vec3(
	//	glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw)),
	//	glm::sin(glm::radians(m_pitch)),
	//	glm::cos(glm::radians(m_pitch)) * glm::sin(glm::radians(m_yaw))));

	//printf("%.2f %.2f %.2f\n", pos().x, pos().y, pos().z);
	setTarget(pos() + m_direction);
}