#include "Clickable.h"
#include <glm/glm/geometric.hpp>

ClickableSelector * Clickable::clickableSelector() const
{
	return m_clickableSelector;
}

Clickable::Clickable(Hitbox *hitbox, const bool clickable, Type type):
	m_hitbox(hitbox),
	m_key(-1), // Just to indicate failed value but I assume that this doesn't happen
	m_clickableSelector(nullptr),
	m_type(type),
	m_clickable(clickable)
{
}

Clickable::~Clickable()
{
	if (m_clickableSelector)
		m_clickableSelector->removeClickable(m_key);
}

void Clickable::setClickableSelector(ClickableSelector & clickableSelector)
{
	m_clickableSelector = &clickableSelector;
	m_clickableSelector->addClickable(this);
}

const Hitbox & Clickable::hitbox() const
{
	return *m_hitbox;
}

void Clickable::onUnselect(Clickable *clickable)
{
}

void Clickable::onClick(const Ray &ray)
{
}

void Clickable::onHover(const Ray &ray)
{
}

ClickableSelector::ClickableSelector():
	m_lastClicked(nullptr),
	m_currentKey(0),
	m_clickCooldown(0)
{
}

void ClickableSelector::addClickable(Clickable * clickable)
{
	unsigned key = 0;
	if (m_freeIndices.empty())
	{
		key = m_currentKey++;
	}
	else
	{
		key = m_freeIndices.top();
		m_freeIndices.pop();
	}
	m_clickables[key] = clickable;

	clickable->m_key = key;
}

void ClickableSelector::removeClickable(unsigned key)
{
	if (m_clickables.count(key) == 1)
	{
		m_freeIndices.push(key);
		m_clickables.erase(key);
	}
}

void ClickableSelector::updateIntersection(const DrawableFactory & drawableFactory, const Input &input, float elapsedTime, const Camera &camera)
{
	const glm::vec3 origin(camera.pos());

	const glm::vec2 mousePos(input.GetMousePos());
	glm::vec3 direction(mousePos, -1);
	
	// Screen to view space
	// (Screen could also be clip. I think that screen does not have a range of -1:1 but not sure)
	// Clip coordinates are -1:1 so just make it a line
	direction = glm::inverse(camera.perspective()) * glm::vec4(direction, 0);
	direction.z = -1.0f;

	// view to world space
	direction = glm::inverse(camera.view()) * glm::vec4(direction, 0);

	const Ray ray(origin, direction,mousePos);

	float min = 0.f;
	Clickable * m_closestClickable = nullptr;
	for (std::map<unsigned, Clickable*>::iterator beg = m_clickables.begin(); beg != m_clickables.end(); beg++)
	{
		if (!beg->second->m_clickable)
			continue;
		const float t = beg->second->m_hitbox->intersect(ray);
		if (t > 0.f)
		{
			if (t < min || !m_closestClickable)
			{
				m_closestClickable = beg->second;
				min = t;
			}
		}
	}

	if (m_closestClickable)
	{
		m_closestClickable->onHover(ray);
		if (m_clickCooldown == 0.f && input.TheMouse.LeftButton)
		{
			if (m_closestClickable != m_lastClicked)
			{
				if(m_lastClicked) m_lastClicked->onUnselect(m_closestClickable);
				m_lastClicked = m_closestClickable;
			}
			m_closestClickable->onClick(ray);
			m_clickCooldown = elapsedTime *2.f;
		}
	}
	else if (m_clickCooldown == 0.f && input.TheMouse.LeftButton)
	{
		if (m_lastClicked) m_lastClicked->onUnselect(nullptr);
		m_lastClicked = nullptr;
		m_clickCooldown = 0.25f;
	}

	m_clickCooldown = glm::clamp(m_clickCooldown - elapsedTime, 0.f,m_clickCooldown);
}