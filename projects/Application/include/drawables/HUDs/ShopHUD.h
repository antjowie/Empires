#pragma once
#include "HUD.h"
#include "DrawableFactory.h"
#include "LineRenderer.h"

#include "drawables/HUDs/VesselBuildHUD.h"

class Planet;
class Vessel;

class ShopHUD : public HUD
{
private:
	Planet *m_planet;
	// For some reason if I won't use pointers, the copy constructor is called for
	// a Clickable object. This copy constructor is implicitly deleted because
	// of the unique pointer for the hitbox. 
	// Making it a pointer solves the problem but I'm not sure why the copy 
	// constructor is called and not the move constructor
	std::vector<std::unique_ptr<VesselBuildHUD>> m_buttons;
	
public:
	ShopHUD(const DrawData &drawData);
	void init(Planet & planet);

	virtual void update(const float elapsedTime) override final;

	virtual void onUnselect(Clickable *clickable) override final;
	virtual void onHover(const Ray &ray) override final;
	virtual void onClick(const Ray &ray) override final;

	virtual void drawTransparent(const Camera &camera) const override final;
};