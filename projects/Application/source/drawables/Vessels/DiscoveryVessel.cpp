#include "drawables/Vessels/DiscoveryVessel.h"
#include "drawables/Planet.h"

DiscoveryVessel::DiscoveryVessel(const DrawData & data):
	Vessel(data),
	m_state(State::Stationary),
	m_targetPlanet(nullptr)
{
	m_name = "Discovery vessel";
}

void DiscoveryVessel::onCreate(Planet & initialPlanet)
{
	m_targetPlanet = &initialPlanet;
}

inline bool smallest(const glm::vec3 &lhs, const glm::vec3 &rhs, const glm::vec3 &pos)
{
	return glm::distance(lhs, pos) < glm::distance(rhs, pos);
}

inline bool correctState(const Planet &planet)
{
	return !(planet.state() == Planet::State::Colonized && planet.colonizingEmpire()->isPlayer());
}

void DiscoveryVessel::update(float elapsedTime)
{
	switch (m_state)
	{
	case DiscoveryVessel::State::Stationary:
	{
		m_status = "Standby";

		// Get closest uncolonized/influenced planet
		// NOTE: should add some query function to get
		// closest influences/uncolonized planet

		std::vector<Planet*> planets;
		for (Planet *planet : m_targetPlanet->planetsInSOI())
		{
			//if ((!closest && correctState(*planet)) || (closest && smallest(planet->pos(), closest->pos(), pos()) && correctState(*planet)))
			if(correctState(*planet))
			{
				planets.push_back(planet);
			}
		}

		//if (closest)
		//{
		//	m_state = State::Travelling;
		//	m_targetPlanet = closest;
		//	setForward(m_targetPlanet->pos() - pos());
		//}

		if (!planets.empty())
		{
			m_targetPlanet = planets[std::rand() % planets.size()];
			setForward(m_targetPlanet->pos() - pos());
			m_state = State::Travelling;
		}
	}
		break;
	case DiscoveryVessel::State::Colonizing:
	{
		m_status = "Colonizing";
		m_targetPlanet->colonize(elapsedTime * 5.f, *m_empire);
		m_lineRenderer->addLine(Line(pos(), m_targetPlanet->pos(), m_empire->color()));

		if (!m_targetPlanet->isEnemyEmpire(*m_empire))
		{
			m_state = State::Stationary;
		}
	}
		break;
	case DiscoveryVessel::State::Travelling:
	{
		m_status = "Travelling";
		move(forward() * elapsedTime * m_speed);

		const float distance = m_targetPlanet->boundsDistance(pos(), scale().x);
		if (distance < 0.f)
		{
			move(forward() * distance);
			m_state = State::Colonizing;
		}
		else
		{
			m_lineRenderer->addLine(Line(pos(), m_targetPlanet->pos(), glm::vec3(0.75f)));
		}
	}
		break;
	}
}
