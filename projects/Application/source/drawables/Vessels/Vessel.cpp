#include "drawables/Vessels/Vessel.h"
#include "drawables/Planet.h"
#include "Empire.h"

#include "hitboxes/SphereHitbox.h"

#undef countof
#include <glm/glm/ext/matrix_transform.hpp>

void Vessel::setForward(const glm::vec3 & forward)
{
	m_forward = glm::normalize(forward);
	m_forward.y = glm::clamp(m_forward.y, -0.99f, 0.99f);
}

const glm::vec3 Vessel::forward() const
{
	return m_forward;
}

Vessel::Vessel(const DrawData & drawData):
	Drawable(drawData),
	Clickable(new SphereHitbox(*this), true, Type::vessel),
	m_forward(0, 0, 1),
	m_up(0,1,0),
	m_lineRenderer(nullptr),
	m_health(1),
	m_empire(nullptr),
	m_speed(1.f),
	m_name("Nameless"),
	m_status("Standby")
{
}

Vessel::~Vessel()
{
	m_empire->removeVessel(*this);
}

void Vessel::init(
	unsigned planetTech,
	Empire & empire, 
	LineRenderer & lineRenderer,
	TextRenderer & textRenderer, 
	const DrawableFactory & drawableFactory)
{
	m_health = planetTech;
	m_empire = &empire;
	m_empire->addVessel(*this);
	m_lineRenderer = &lineRenderer;

	m_vesselHUD = drawableFactory.createDrawable<VesselHUD>("hud");
	m_vesselHUD->setClickableSelector(*clickableSelector());
	m_vesselHUD->setTextRenderer(textRenderer);
	m_vesselHUD->setVessel(*this);
}

void Vessel::onCreate(Planet & initialPlanet)
{
}

void Vessel::update(float elapsedTime)
{
}

void Vessel::onUnselect(Clickable * clickable)
{
	m_vesselHUD->m_clickable = false;
	m_health = 0;
}

void Vessel::onClick(const Ray & ray)
{
	m_vesselHUD->m_clickable = true;
}

bool Vessel::isDead() const
{
	return m_health <= 0;
}

void Vessel::damage(unsigned damage)
{
	m_health -= damage;
}

void Vessel::drawTransparent(const Camera & camera) const
{
	m_vesselHUD->drawTransparent(camera);
}

glm::mat4 Vessel::toWorld() const
{
	const glm::mat4 tTrans = glm::translate(glm::mat4(1), pos());
	const glm::mat4 tScale = glm::scale(glm::mat4(1), scale());

	glm::mat4 tRotation(1);
	//tRotation = glm::lookAt(glm::vec3(0, 0, 0), m_forward * glm::vec3(1,1,-1), m_up);

	glm::vec3 forward = glm::normalize(m_forward * -1.f);
	glm::vec3 side = glm::normalize(glm::cross(m_up, forward));
	glm::vec3 up = glm::normalize(glm::cross(forward, side));
	
	tRotation[0] = glm::vec4(side,0);
	tRotation[1] = glm::vec4(up, 0);
	tRotation[2] = -glm::vec4(forward, 0);

	return tTrans * tRotation * tScale;
}