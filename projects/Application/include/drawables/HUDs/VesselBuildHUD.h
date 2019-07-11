#pragma once
#include "HUD.h"
#include "drawables/Vessels/DiscoveryVessel.h"
#include "drawables/Vessels/SupplyVessel.h"
#include "drawables/Vessels/CarrierVessel.h"
#include "drawables/Vessels/FighterVessel.h"

class VesselBuildHUD : public HUD
{
private:
	std::string m_displayName;
	
	TextRenderer &m_textRenderer;
	bool m_isHovering;

	const glm::vec4 m_defaultColor;
	const glm::vec4 m_hoverColor;

public:
	VesselBuildHUD(const DrawData &drawData, TextRenderer &textRenderer, const char * displayName);

	virtual void update(const float elaspedTime);

	virtual void onHover(const Ray &ray) override final;

	// This is never called but to stay consistent
	virtual void drawTransparent(const Camera &camera) const override final;

	std::string m_name;
};