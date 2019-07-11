#include "drawables/HUDs/VesselHUD.h"
#include "drawables/Vessels/Vessel.h"
#include "Empire.h"

VesselHUD::VesselHUD(const DrawData &drawData) :
	HUD(drawData, false),
	m_vessel(nullptr)

{
	setColor(glm::vec4(0.f, 0.f, 0.f, 0.5f));

	setPos(glm::vec3(0, -0.75f, -0.1f));
	setScale(glm::vec3(1, 0.25f, 1));
}

void VesselHUD::setVessel(Vessel &vessel)
{
	m_vessel = &vessel;
}

void VesselHUD::setTextRenderer(TextRenderer & textRenderer)
{
	m_textRenderer = &textRenderer;
}


void VesselHUD::onUnselect(Clickable *clickable)
{
	m_clickable = false;
}

void VesselHUD::onClick(const Ray &ray)
{
	m_clickable = true;
}

void VesselHUD::drawTransparent(const Camera & camera) const
{
	if (!m_clickable)
		return;

	HUD::drawTransparent(camera);

	m_textRenderer->drawText(Text(
		m_vessel->m_name,
		glm::vec3(0, 0.15f, 0) + pos(),
		true, true, glm::vec4(1), 0.15f));

	m_textRenderer->drawText(Text(
		"Health " + std::to_string(m_vessel->m_health),
		glm::vec3(-0.45f, 0.05f, 0) + pos(),
		false, true, glm::vec4(1.f), 0.10f));

	m_textRenderer->drawText(Text(
		"Empire " + m_vessel->m_empire->name(),
		glm::vec3(-0.45f, -0.05f, 0) + pos(),
		false, true, glm::vec4(m_vessel->m_empire->color(),1.f), 0.10f));

	m_textRenderer->drawText(Text(
		m_vessel->m_status,
		glm::vec3(-0.45f, -0.15f, 0) + pos(),
		false, true, glm::vec4(1.f), 0.10f));
}