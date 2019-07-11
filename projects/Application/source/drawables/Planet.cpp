#include "drawables/Planet.h"
#include "hitboxes/SphereHitbox.h"
#include <algorithm>

void Planet::updateColonizationProgress(float elapsedTime, Empire &colonizingEmpire, State stateWhenComplete, float modifier)
{
	// A different empire is colonizing
	if (&colonizingEmpire != m_colonizingEmpire)
	{
		m_colonizingProgress = 0;
		m_colonizingEmpire = &colonizingEmpire;
	}

	constexpr float timeToColonize = 10.f;
	constexpr float techPower = 1.5f;

	m_colonizingProgress +=
		elapsedTime /
		timeToColonize /
		glm::pow(static_cast<float>(m_tech), techPower) *
		modifier;

	if (m_colonizingProgress > 1.f)
		m_colonizingProgress = 1.f;

	// Lerp
	m_color = glm::vec3(1) * (1.f - m_colonizingProgress) + m_colonizingEmpire->color() * m_colonizingProgress;

	if (m_colonizingProgress == 1.f)
	{
		m_colonizingProgress = 0; // Redundant but why not
		setEmpire(*m_colonizingEmpire);
		m_state = stateWhenComplete;
	}
}

Planet::Planet(const DrawData &drawdata) :
	Drawable(drawdata),
	Clickable(new SphereHitbox(*this),true, Type::planet),	
	// Optimzation related
	//std::vector<Planet*> m_planetsInSOI;
	m_planets(nullptr),
	m_planetsLoaded(false),
	m_planetCheckTime(0),
	m_shouldInfluence(true),

	// Stats
	m_state(State::Uncolonized),
	m_name("No name"),
	m_tech(1),
	m_resource(1),
	m_credits(0),

	m_colonizingProgress(0),
	m_colonizingEmpire(nullptr),
	m_empire(nullptr),

	// Fighter stuff
	m_fighterCooldown(0),
	m_vesselsToAdd(nullptr),
	//std::vector<FighterVessel*> m_fighters;

	// Flags
	m_hover(false),
	m_drawName(false),
	
	m_textRenderer(nullptr),
	m_lineRenderer(nullptr)
{
}

void Planet::init(
	const glm::vec3 &position, 
	const std::string & name, 
	unsigned tech, unsigned resource, 
	LineRenderer &lineRenderer, 
	TextRenderer &textRenderer,
	const DrawableFactory &drawableFactory,
	std::vector<std::unique_ptr<Vessel>> &vesselsToAdd)
{
	assert(clickableSelector());

	m_vesselsToAdd = &vesselsToAdd;
	m_lineRenderer = &lineRenderer;
	m_textRenderer = &textRenderer;
	m_factory = &drawableFactory;
	setPos(position);

	m_name = name;
	m_tech = glm::clamp(tech, 1u, 15u);
	setScale(4.f + static_cast<float>(m_tech) / 3.f);

	m_resource = glm::clamp(resource, 1u, 15u);
	
	m_soi = drawableFactory.createDrawable<SOI>("soi");
	m_soi->init(pos(), scale().x);

	// Create HUD
	m_shopHUD = m_factory->createDrawable<ShopHUD>("hud");
	m_shopHUD->setClickableSelector(*clickableSelector());
	m_shopHUD->init(*this);
}

void Planet::setPlanetsVector(const std::vector<std::unique_ptr<Planet>>& planets)
{
	m_planets = &planets;
}

void Planet::update(float elapsedTime)
{
	//if (!m_fighters.empty())
	//{
	//	auto i = std::remove_if(m_fighters.begin(), m_fighters.end(), [](FighterVessel* fighter) {return fighter->isDead(); });
	//	if (i != m_fighters.end())
	//	{
	//		m_fighters.erase(i, m_fighters.end());
	//	}
	//}

	// Update name
	m_drawName = false;
	if (m_hover)
		m_drawName = true;
	m_hover = false;

	m_shopHUD->update(elapsedTime);

	// Else game will lag
	m_planetCheckTime += elapsedTime;
	if (m_planetCheckTime > 10.f)
	{
		bool uncolonized = false;
		for (Planet * planet : m_planetsInSOI)
		{
			if (planet->state() == State::Uncolonized)
				uncolonized = true;
		}
		if (uncolonized)
			m_shouldInfluence = true;
		else
			m_shouldInfluence = false;
		m_planetCheckTime = 0.f;
	}

	if (m_shouldInfluence)
	{
		// Actual influencing stuff
		if (m_state == State::Colonized || m_state == State::Influenced)
		{
			for (Planet * planet : m_planetsInSOI)
			{		
				planet->influence(elapsedTime, *m_empire);
			}
		}
	}

	if(m_state == State::Colonized)
		m_credits += glm::clamp(elapsedTime * (float)m_tech, 0.f, (float)m_resource);

	// Keep creating fighter vessels till cap
	m_fighterCooldown -= elapsedTime;
	if (m_fighterCooldown < 0) m_fighterCooldown = 0;
	if (m_credits > 25.f && m_fighterCooldown == 0)
	{
		createVessel(VesselType::Fighter);
		m_fighterCooldown = 10.f - float(m_tech) * 0.5f;
	}
}

void Planet::fillPlanetsInSOI()
{
	assert(m_planets);
	if (m_planetsLoaded)
		return;

	for (const std::unique_ptr<Planet> &planet : *m_planets)
	{
		if (planet.get() == this)
			continue;

		if (m_soi->contains(planet->pos()))
			m_planetsInSOI.push_back(planet.get());
	}

	m_planetsLoaded = true;
}

void Planet::setHudClickable(bool clickable)
{
	m_shopHUD->m_clickable = clickable;
}

void Planet::setEmpire(Empire & empire)
{
	if (m_empire)
	{
		m_empire->removePlanet(*this);
	}

	m_empire = &empire;
	m_empire->addPlanet(*this);
	
	m_color = m_empire->color();

	if (!m_soi)
	{
		std::printf("SOI for %s (located at %.2f, %.2f) has not yet been created. SOI may have incorrect color", m_name, pos().x, pos().y);
	}
	else
	{
		m_soi->setColor(m_empire->color());
	}

	m_state = State::Colonized;
	m_colonizingEmpire = m_empire;
	fillPlanetsInSOI();
}

void Planet::influence(float elapsedTime, Empire & influencingEmpire)
{
	switch (m_state)
	{
	case Planet::State::Uncolonized:
		m_state = State::Influencing;

		break;

	case Planet::State::Influencing:
		updateColonizationProgress(elapsedTime, influencingEmpire, State::Influenced, 0.25f);

		break;
	}
}

void Planet::colonize(float elapsedTime, Empire &colonizingEmpire)
{
	switch (m_state)
	{
	case Planet::State::Colonized:
		if (&colonizingEmpire != m_colonizingEmpire)
		{
			//m_colonizingProgress = 0;
			//m_colonizingEmpire = &colonizingEmpire;
			m_state = State::Uncolonized;
		}

		break;

	case Planet::State::Uncolonized:
	case Planet::State::Influencing:
		m_state = State::Colonizing;

		break;

	case Planet::State::Colonizing:
		updateColonizationProgress(elapsedTime, colonizingEmpire, State::Colonized, 1.f);

		break;

	case Planet::State::Influenced:
		if (&colonizingEmpire == m_colonizingEmpire)
		{
			m_colonizingProgress = 1.f;
			// This just colonizes the planet, but there is also some color changing going on
			// so in a way this avoids code duplication
			updateColonizationProgress(elapsedTime, colonizingEmpire, State::Colonized, 1.f);
		}

		break;
	}

}

bool Planet::isEnemyEmpire(Empire & empire) const
{
	return &empire != m_empire;
}

Empire * Planet::colonizingEmpire() const
{
	return m_colonizingEmpire;
}

float Planet::credits() const
{
	return m_credits;
}

const glm::vec3 & Planet::color() const
{
	return m_color;
}

SOI * Planet::soi() const
{
	return m_soi.get();
}

Planet::State Planet::state() const
{
	return m_state;
}

const std::vector<Planet*>& Planet::planetsInSOI() const
{
	return m_planetsInSOI;
}

float Planet::boundsDistance(const glm::vec3 & pos, float scale) const
{
	const float distance = glm::distance(pos, this->pos());
	const float bracing = this->scale().x + scale + 1.f;
	
	return distance - bracing;
}

std::vector<FighterVessel*> Planet::requestFighters(const glm::vec3 &pos, size_t count)
{
	assert(false);
	std::vector<FighterVessel*> fighters;
	fighters.insert(std::begin(fighters), std::begin(m_fighters), std::end(m_fighters));

	if (fighters.size() > count)
		fighters.erase(fighters.begin() + count, fighters.end());

	for (FighterVessel * fighter : m_fighters)
	{
		fighter->request(pos);
	}

	return fighters;
}

std::vector<FighterVessel*> invade(std::vector<FighterVessel*> &vessels)
{
	// Do some fighting
	assert(false);
	return {};
}

void Planet::createVessel(VesselType type)
{
	if (m_credits < 10.f)
		return;
	m_credits -= 10.f;
	switch (type)
	{
	case Planet::VesselType::Fighter:
	{
		if (m_fighters.size() + 1 > m_resource)
		{
			m_credits += 10.f;
			return;
		}

		m_vesselsToAdd->push_back(m_factory->createDrawable<FighterVessel>("fv"));

		m_fighters.push_back(static_cast<FighterVessel*>(m_vesselsToAdd->back().get()));
	}
	break;

	case Planet::VesselType::Carrier:
		m_vesselsToAdd->push_back(m_factory->createDrawable<CarrierVessel>("cv"));
		break;
	case Planet::VesselType::Supply:


		m_vesselsToAdd->push_back(m_factory->createDrawable<SupplyVessel>("sv"));
		break;
	case Planet::VesselType::Discovery:
		m_vesselsToAdd->push_back(m_factory->createDrawable<DiscoveryVessel>("dv"));
		break;
	default:
		break;
	}

	Vessel *v = m_vesselsToAdd->back().get();
	v->setClickableSelector(*clickableSelector());
	v->init(m_tech, *m_empire, *m_lineRenderer, *m_textRenderer, *m_factory);

	v->setPos(pos());
	v->onCreate(*this);


}

void Planet::draw(const Camera & camera) const
{
	Drawable::draw(camera);
	if (m_drawName)
	{
		m_textRenderer->drawText(Text(
			m_name,
			pos() + glm::vec3(0, scale().y + 0.1f, 0),
			true));
	}
}

void Planet::drawTransparent(const Camera & camera) const
{
	// Drawing so many sois aint a good idea
	if (m_drawName)// || m_state == State::Colonized || m_state == State::Influenced)
	{
		m_soi->drawTransparent(camera); 
	}
	m_shopHUD->drawTransparent(camera);
}

void Planet::onUnselect(Clickable *clickable)
{
	m_shopHUD->m_clickable = false;
}

void Planet::onClick(const Ray &ray)
{
	m_shopHUD->m_clickable = true;
}

void Planet::onHover(const Ray &ray)
{
	m_hover = true;
}

SOI::SOI(const DrawData & drawData):
	Drawable(drawData),
	m_color(0),
	m_hitbox(*this)
{
}

void SOI::init(const glm::vec3 & pos, float planetScale, const glm::vec3 & color)
{
	setPos(pos);
	setScale(2.f * planetScale * 5.f + 10.f);
	setColor(color);
}

void SOI::setColor(const glm::vec3 & color)
{
	m_color = glm::vec4(color,0.25f);
}

const glm::vec4 & SOI::color() const
{
	return m_color;
}

bool SOI::contains(const glm::vec3 & point)
{
	return m_hitbox.contains(point);
}

void SOI::draw(const Camera & camera) const
{
}

void SOI::drawTransparent(const Camera & camera) const
{
	// It's basically a drawable draw call but with an additional color 
	glBindBuffer(GL_ARRAY_BUFFER, m_drawData.vbo);
	glUseProgram(m_drawData.program);

	const glm::mat4 mvp = camera.perspective() * camera.view() * toWorld();
	//const glm::mat4 mvp(1);
	glUniformMatrix4fv(glGetUniformLocation(m_drawData.program, "mvp"), 1, GL_FALSE, &mvp[0][0]);

	glUniform4fv(glGetUniformLocation(m_drawData.program, "color"), 1, &m_color[0]);

	glVertexAttribPointer(
		m_drawData.posLoc,
		3,
		GL_FLOAT,
		GL_FALSE,
		5 * sizeof(GLfloat),
		0
	);

	glEnableVertexAttribArray(m_drawData.posLoc);

	glDrawArrays(GL_TRIANGLES, 0, m_drawData.indicesCount);
}