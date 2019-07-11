#include "Empire.h"

#include "drawables/Planet.h"
#include "LineRenderer.h"

#include <algorithm>

Empire::Empire():
	m_homePlanet(nullptr),
	m_color(0),
	m_isPlayer(false)
{
	static int count = 0;
	m_name = std::string("Temp Empire #").append(std::to_string(count++));
}

void Empire::setColor(const glm::vec3 & color)
{
	m_color = color;
}

const glm::vec3 & Empire::color() const
{
	return m_color;
}

void Empire::setName(const char * name)
{
	m_name = name;
}

const std::string & Empire::name() const
{
	return m_name;
}

void Empire::setHomePlanet(Planet & planet)
{
	m_homePlanet = &planet;
}

const Planet * Empire::homePlanet() const
{
	return m_homePlanet;
}

const TSP & Empire::tsp() const
{
	return m_tsp;
}

void Empire::addPlanet(Planet & planet)
{
	m_planets.push_back(&planet);
	// This is very slow when empire is growing rapidly
	// Time complexity is n^2 but is happens for around 1000
	// planets which causes a huge slowdown
	// Some random value
	if (glm::distance(m_homePlanet->pos(), planet.pos()) < 250.f)
	{
		m_surroundingPlanets.push_back(&planet);
		m_tsp.createWeightMatrix(m_surroundingPlanets);
	}
}

void Empire::removePlanet(Planet & planet)
{
	m_planets.erase(std::remove(m_planets.begin(), m_planets.end(), &planet), m_planets.end());
	// Some random thingy
	if (glm::distance(m_homePlanet->pos(), planet.pos()) < 250.f)
	{
		m_surroundingPlanets.push_back(&planet);
		m_tsp.createWeightMatrix(m_surroundingPlanets);
	}
}

void Empire::addVessel(Vessel & vessel)
{
	m_vessels.push_back(&vessel);
}

void Empire::removeVessel(Vessel & vessel)
{
	m_vessels.erase(std::remove(m_vessels.begin(), m_vessels.end(), &vessel), m_vessels.end());
}

const std::vector<Planet*>& Empire::planets() const
{
	return m_planets;
}

const std::vector<Planet*>& Empire::surroundingPlanets() const
{
	return m_surroundingPlanets;
}

void Empire::markAsPlayer()
{
	m_isPlayer = true;
}

bool Empire::isPlayer() const
{
	return m_isPlayer;
}

void Empire::emitLine(LineRenderer &lineRenderer) const
{
	if (m_homePlanet)
		lineRenderer.addLine(Line(
			m_homePlanet->pos(),
			m_homePlanet->pos() + glm::vec3(0, 1e10, 0),
			m_color
		));
}
