#include "drawables/Vessels/FighterVessel.h"
#include "drawables/Planet.h"

FighterVessel::FighterVessel(const DrawData & data):
	Vessel(data),
	m_state(State::Defending),
	m_targetPlanet(nullptr)
{
}

void FighterVessel::request(const glm::vec3 & pos)
{
	m_state = State::Transferring;
	setForward(pos - this->pos());
	m_status = "Transferring";
}

void FighterVessel::release(Planet & targetPlanet)
{
	m_state = State::Fighting;
	m_targetPlanet = &targetPlanet;
	m_clickable = true;
}

void FighterVessel::onCreate(Planet & initialPlanet)
{
	m_targetPlanet = &initialPlanet;
	m_name = "Fighter vessel";
	m_status = "Repositioning";
	
	m_state = State::Positioning;
	m_targetPos = glm::vec3(std::rand()% 20 - 10, std::rand() % 20 - 10, std::rand() % 20 - 10);
	m_targetPos = glm::normalize(m_targetPos);
	setForward(m_targetPos);
	m_targetPos = m_targetPlanet->pos() + m_targetPos * -m_targetPlanet->boundsDistance(pos(), scale().x);
}

void FighterVessel::update(float elapsedTime)
{
	switch (m_state)
	{
	case FighterVessel::State::Defending:
	{
	}
	break;
	case FighterVessel::State::Positioning:
	{
		move(forward() * m_speed * elapsedTime);

		const float distance = m_targetPlanet->boundsDistance(pos(),scale().x);
		if (distance > 0.f)
		{
			move(forward() * -distance);

			m_state = State::Defending;
			m_status = "Defending";
			setForward(forward() * glm::vec3(1, 0, 1));
		}
	}
	case FighterVessel::State::Docked:
	{

	}
	break;
	case FighterVessel::State::Transferring:
	{
		move(forward() * elapsedTime * m_speed);

		if (glm::dot(forward(), m_targetPos - pos()) < 0.f)
		{
			m_state = State::Docked;
			m_status = "Docked";
			m_clickable = false;
		}
	}
	break;
	case FighterVessel::State::Fighting:
	{
	}
	break;
	}
}

void FighterVessel::draw(const Camera & camera) const
{
	if (m_state != State::Docked)
		Vessel::draw(camera);
}