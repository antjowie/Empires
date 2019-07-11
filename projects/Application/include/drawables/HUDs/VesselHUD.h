#pragma once
#include "HUD.h"

#include "TextRenderer.h"

class Vessel;

class VesselHUD : public HUD
{
private:
	Vessel *m_vessel;

	TextRenderer *m_textRenderer;

public:
	VesselHUD(const DrawData &drawData);

	void setVessel(Vessel &vessel);
	void setTextRenderer(TextRenderer &textRenderer);

	virtual void onUnselect(Clickable *clickable) override final;
	virtual void onClick(const Ray &ray) override final;

	virtual void drawTransparent(const Camera &camera) const override final;
};