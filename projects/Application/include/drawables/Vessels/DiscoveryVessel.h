#pragma once
#include "Vessel.h"

class DiscoveryVessel : public Vessel
{
private:
	enum class State
	{
		Stationary,
		Colonizing,
		Travelling
	};

	State m_state;
	Planet * m_targetPlanet;

public:
	DiscoveryVessel(const DrawData &data);

	virtual void onCreate(Planet &initialPlanet) override final;
	virtual void update(float elapsedTime) override final;
};
