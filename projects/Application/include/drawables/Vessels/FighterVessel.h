#pragma once
#include "Vessel.h"

class FighterVessel : public Vessel
{
private:
	enum class State
	{
		Defending,
		Positioning,
		Transferring,
		Docked,
		Fighting
	};
	
	State m_state;
	Planet * m_targetPlanet;
	
	glm::vec3 m_targetPos;

public:
	FighterVessel(const DrawData &data);
	
	void request(const glm::vec3 &pos);
	void release(Planet &targetPlanet);

	virtual void onCreate(Planet &initialPlanet) override final;
	virtual void update(float elapsedTime) override final;

	virtual void draw(const Camera &camera) const override final;
};