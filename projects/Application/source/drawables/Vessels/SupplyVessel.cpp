#include "drawables/Vessels/SupplyVessel.h"
#include "drawables/Planet.h"
#include "Empire.h"
#include <algorithm>

SupplyVessel::SupplyVessel(const DrawData & data):
	Vessel(data),
	m_targetPlanet(nullptr),
	m_credits(0)
{
	m_name = "Supply vessel";
}

void SupplyVessel::onCreate(Planet & initialPlanet)
{
	m_state = State::Depositing;
	m_targetPlanet = &initialPlanet;
}

void SupplyVessel::update(float elapsedTime)
{
	switch (m_state)
	{
	case SupplyVessel::State::Travelling:
	{
		m_status = "Travelling";
		move(forward() * m_speed * elapsedTime);

		const float distance = m_targetPlanet->boundsDistance(pos(), scale().x);
		if (distance < 0.f)
		{
			move(forward() * distance);
			if (m_currentTarget < m_toVisit.size())
				m_state = State::Taxing;
			else
			{
				m_toVisit.clear();
				m_state = State::Depositing;
			}
		}
	}
		break;
	
	case SupplyVessel::State::Taxing:
	{
		m_status = "Taxing"; 
		// Do the taxing
		// but not yet
		m_currentTarget++;
		
		if (m_currentTarget < m_toVisit.size())
		{
			m_targetPlanet = m_toVisit[m_currentTarget];
		}
		else
		{
			m_targetPlanet = m_empire->homePlanet();
		}

		setForward(m_targetPlanet->pos() - pos());
		m_state = State::Travelling;
	}
		break;

	case SupplyVessel::State::Depositing:
	{		
		m_status = "Depositing";
		if (m_credits == 0)
		{
			auto visit = m_empire->surroundingPlanets();
			visit.erase(std::find(visit.begin(), visit.end(), m_empire->homePlanet()));

			if (!visit.empty())
			{

			m_currentTarget = 1;

			const Planet *home = m_empire->homePlanet();
			std::sort(std::begin(visit), std::end(visit), 
				[&home](const Planet *a, const Planet *b) -> bool {return glm::distance(a->pos(), home->pos()) < glm::distance(b->pos(), home->pos()); });
			if(visit.size() > 4)
				visit.erase(visit.begin() + 4, visit.end());

			m_toVisit.clear();
			m_toVisit = m_empire->tsp().shortestPath(*m_targetPlanet, *m_empire->homePlanet(), visit);

			m_targetPlanet = m_toVisit[m_currentTarget];
			setForward(m_targetPlanet->pos() - pos());
			m_state = State::Travelling;
			}
			else
			{
				m_status = "Standby";
			}
		}
	}
		break;
	}


	// Do the drawing stuff
	if (!m_toVisit.empty())
	{
		m_lineRenderer->addLine(Line(pos(), m_toVisit[m_currentTarget]->pos(), glm::vec3(0.3f)));
		for (size_t i = m_currentTarget; i < m_toVisit.size() - 1; i++)
			m_lineRenderer->addLine(Line(m_toVisit[i]->pos(), m_toVisit[i + 1]->pos(), glm::vec3(0.3f)));
	}
}