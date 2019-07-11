#include "drawables/Vessels/CarrierVessel.h"
#include "drawables/Planet.h"

CarrierVessel::CarrierVessel(const DrawData & data):
	Vessel(data)
{
}

void CarrierVessel::onCreate(Planet & initialPlanet)
{
	m_name = "Carrier vessel";
	m_status = "Requesting fighters";
	m_targetPlanet = &initialPlanet;
	m_cap = 10;
	setScale(3);

	setForward(glm::vec3(1, 0, 1));
	setPos(m_targetPlanet->pos() + forward() * 5.f);

	m_state = State::Travelling;
}

void CarrierVessel::update(float elapsedTime)
{
	switch (m_state)
	{
	case CarrierVessel::State::Requesting:
	{
		for (FighterVessel *fighter : m_targetPlanet->requestFighters(pos(),m_cap - m_fighters.size()))
		{
			fighter->request(pos());
		}

		bool done = false;
		for (FighterVessel *fighter : m_fighters)
		{
			if (!fighter->m_clickable)
				done = true;
		}

		if (done)
		{
			m_state = State::Standby;
		}
	}
	break;
	case CarrierVessel::State::Regrouping:
	{
		m_status = "Regrouping";
		m_state = State::Requesting;
	}
	break;
	case CarrierVessel::State::Dropping:
	{
		m_status = "Fighting";
		//m_targetPlanet->
	}
	break;
	case CarrierVessel::State::Travelling:
	{
		m_status = "Travelling";
		move(forward() * elapsedTime* m_speed);

		const float distance = m_targetPlanet->boundsDistance(pos(), scale().x * 2.f);
		if (distance < 0.f)
		{
			move(forward() * distance);

			if (m_targetPlanet->state() == Planet::State::Uncolonized)
				m_state = State::Standby;
			else if (m_targetPlanet->isEnemyEmpire(*m_empire))
			{
				const auto toAdd = m_targetPlanet->requestFighters(pos(), m_cap - m_fighters.size());
				m_fighters.insert(std::end(m_fighters), std::begin(toAdd), std::end(toAdd));
				m_status = "Requesting";
				m_state = State::Requesting;
			}
			else
			{
				//m_targetPlanet->invade(m_fighters);
				for (FighterVessel* fighter : m_fighters)
				{
					fighter->release(*m_targetPlanet);
				}
				m_state = State::Dropping;
			}
		}
	}
	break;
	case CarrierVessel::State::Standby:
	{
		m_status = "Standby";
	}
	break;
	}
}
