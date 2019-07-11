#pragma once
#include "Drawable.h"
#include "Clickable.h"

#include "drawables/HUDs/VesselHUD.h"
#include <memory>

class Planet;
class LineRenderer;
class Empire;

class Vessel : public Drawable, public Clickable
{
private:
	glm::vec3 m_forward;
	glm::vec3 m_up;

	std::unique_ptr<VesselHUD> m_vesselHUD;

	int m_health;

protected:
	friend VesselHUD;

	LineRenderer *m_lineRenderer;
	Empire *m_empire;
	float m_speed;

	// Member var is not protected cuz I have to do some 
	// data verification
	void setForward(const glm::vec3 &forward);
	const glm::vec3 forward() const;

public:
	std::string m_name;
	std::string m_status;

	Vessel(const DrawData &drawData);
	virtual ~Vessel() override;

	void init(
		unsigned planetTech,
		Empire &empire, 
		LineRenderer &lineRenderer,
		TextRenderer &textRenderer,
		const DrawableFactory &drawableFactory);

	virtual void onCreate(Planet &initialPlanet);
	virtual void update(float elapsedTime);	
	
	virtual void onUnselect(Clickable *clickable) override final;
	virtual void onClick(const Ray &ray) override final;

	bool isDead() const;
	void damage(unsigned damage);

	virtual void drawTransparent(const Camera &camera) const override;
	virtual glm::mat4 toWorld() const override;
};