#pragma once
#include "Vessel.h"

class SupplyVessel : public Vessel
{
private:
	enum class State
	{
		Travelling,
		Taxing,
		Depositing
	};
	State m_state;

	const Planet *m_targetPlanet;
	float m_credits;

	std::vector<Planet*> m_toVisit;
	size_t m_currentTarget;

public:
	SupplyVessel(const DrawData &data);

	virtual void onCreate(Planet & initialPlanet) override final;
	virtual void update(float elapsedTime) override final;
};