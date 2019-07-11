#pragma once
#include "Drawable.h"
#include "Clickable.h"
#include "TextRenderer.h"
#include "Camera.h"

class HUD : public Drawable, public Clickable
{
private:
	glm::vec4 m_color;

public:
	HUD(const DrawData &drawData, const bool isClickable = false);

	void setColor(const glm::vec4 &color);

	virtual void update(const float elapsedTime);
	virtual void draw(const Camera &camera) const override final;
	virtual void drawTransparent(const Camera &camera) const override;
};