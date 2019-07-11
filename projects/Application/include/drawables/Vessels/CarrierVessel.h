#pragma once
#include "Vessel.h"

class FighterVessel;

class CarrierVessel : public Vessel
{
private:
	enum class State
	{
		Requesting,
		Regrouping,
		Dropping,
		Travelling,
		Standby
	};

	State m_state;
	Planet *m_targetPlanet;
	std::vector<FighterVessel*> m_fighters;
	size_t m_cap;

public:
	CarrierVessel(const DrawData &data);

	void virtual onCreate(Planet &initialPlanet) override final;
	void virtual update(float elapsedTime) override final;
};