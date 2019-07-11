#include "drawables/HUDs/VesselBuildHUD.h"

VesselBuildHUD::VesselBuildHUD(const DrawData & drawData, TextRenderer &textRenderer, const char * displayName):
	HUD(drawData),
	m_displayName(displayName),
	m_textRenderer(textRenderer),
	m_isHovering(false),
	m_defaultColor(glm::vec4(0.5f, 0.5f, 0.5f, 1)),
	m_hoverColor(glm::vec4(0.25f, 0.25f, 0.25f,1))
{
	//setColor(glm::vec4(1,1,1,0.25f));
	setColor(m_defaultColor);
}

void VesselBuildHUD::update(const float elaspedTime)
{
	if (m_isHovering)
		setColor(m_hoverColor);

	else
		setColor(m_defaultColor);
	m_isHovering = false;
}

void VesselBuildHUD::onHover(const Ray & ray)
{
	m_isHovering = true;
	setColor(m_hoverColor);
	//setColor(glm::vec4(1,1,1,0.75f));
}

void VesselBuildHUD::drawTransparent(const Camera & camera) const
{
	HUD::drawTransparent(camera);
	m_textRenderer.drawText(Text(
		m_displayName, pos(),
		true, true, glm::vec4(0.8f), 0.08f));
}