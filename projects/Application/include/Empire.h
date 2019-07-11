#pragma once
#include <vector>

#include <glm/glm/vec3.hpp>
#include <string>

#include "TSP.h"

class Planet;
class Vessel;
class LineRenderer;

class Empire
{
private:
	std::vector<Planet*> m_surroundingPlanets;
	std::vector<Planet*> m_planets;
	std::vector<Vessel*> m_vessels;

	TSP m_tsp;

	Planet *m_homePlanet;
	glm::vec3 m_color;
	std::string m_name;

	bool m_isPlayer;

public:
	Empire();

	void setHomePlanet(Planet& planet);
	const Planet *homePlanet() const;

	const TSP &tsp() const;

	void setColor(const glm::vec3 &color);
	void setName(const char* name);	
	const glm::vec3 &color() const;
	const std::string& name() const;

	// These functions are called by their classes
	// you should not call them manually but instead
	// have the setEmpire functions call them
	void addPlanet(Planet &planet);
	void addVessel(Vessel &vessel);
	void removePlanet(Planet &planet);
	void removeVessel(Vessel &vessel);

	const std::vector<Planet*> &planets() const;
	const std::vector<Planet*> &surroundingPlanets() const;
	//const std::vector<Vessel*> &vessels() const;

	void markAsPlayer();
	bool isPlayer() const;

	void emitLine(LineRenderer &lineRenderer) const; 
};