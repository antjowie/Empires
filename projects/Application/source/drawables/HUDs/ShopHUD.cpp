#include "drawables/HUDs/ShopHUD.h"
#include "drawables/Planet.h"

ShopHUD::ShopHUD(const DrawData &drawData):
	HUD(drawData,false),
	m_planet(nullptr)
{
	setColor(glm::vec4(0.f, 0.f, 0.f, 0.5f));
	
	setPos(glm::vec3(0, -0.75f, -0.1f));
	setScale(glm::vec3(1, 0.25f, 1));
}
void ShopHUD::init(Planet & planet)
{
	m_planet = &planet;

	// Quite an odd place but otherwise I have to add another function to the class
	// and I'm too lazy for that
	int i = 0;
	for (float y = 0.f; y < 2.f; y++)
		for (float x = 0.f; x < 2.f; x++)
		{
			std::string title;

			switch (i)
			{
			case 0:
				title += 'd';
				break;
			case 1:
				title += 'f';
				break;
			case 2:
				title += 's';
				break;
			case 3:
				title += 'c';
				break;
			}
			i++;

			title += 'v';
			m_buttons.push_back(std::make_unique<VesselBuildHUD>(m_drawData, *m_planet->m_textRenderer, title.c_str()));
			VesselBuildHUD &vessel = *m_buttons.back();
			vessel.setScale(glm::vec3(0.1f, 0.1f, 1.f));
			
			glm::vec2 posOffset(0.5f + vessel.scale().x, -0.525f - vessel.scale().y);
			posOffset += glm::vec2(
				x *  (vessel.scale().x * 2.f + 0.025f),
				y * -(vessel.scale().y * 2.f + 0.05f));

			vessel.setPos(glm::vec3(posOffset, -0.1f));
			vessel.m_name = title;
		}
}

void ShopHUD::update(const float elapsedTime)
{
	if (!m_planet->isEnemyEmpire(*m_planet->m_empire))
	{
		for (auto &button : m_buttons)
		{
			button->update(elapsedTime);
		}
	}
}

void ShopHUD::onUnselect(Clickable *clickable)
{
	m_clickable = false;
}

void ShopHUD::onHover(const Ray & ray)
{
	if (!m_planet->isEnemyEmpire(*m_planet->m_empire))
	{
		for (auto &button : m_buttons)
		{
			if (button->hitbox().intersect(ray) > 0.f)
				button->onHover(ray);
		}
	}
}

void ShopHUD::onClick(const Ray &ray)
{
	m_clickable = true;
	if (!m_planet->isEnemyEmpire(*m_planet->m_empire))
	{
		for (const auto &button : m_buttons)
		{
			if (button->hitbox().intersect(ray) > 0.f)
			{	
				//m_planet->m_vesselsToAdd->push_back(button->createVessel(*m_planet->m_factory));
				if (button->m_name == "dv")
					m_planet->createVessel(Planet::VesselType::Discovery);
				if (button->m_name == "fv")
					m_planet->createVessel(Planet::VesselType::Fighter);
				if (button->m_name == "sv")
					m_planet->createVessel(Planet::VesselType::Supply);
				if (button->m_name == "cv")
					m_planet->createVessel(Planet::VesselType::Carrier);
			}
		}
	}
}

void ShopHUD::drawTransparent(const Camera & camera) const
{
	if (!m_clickable)
		return;

	// Render frame
	HUD::drawTransparent(camera);

	// Render planet stats
	m_planet->m_textRenderer->drawText(Text(
		m_planet->m_name,
		glm::vec3(0, 0.15f, 0) + pos(),
		true, true, glm::vec4(1), 0.15f));

	m_planet->m_textRenderer->drawText(Text(
		std::string("Tech " + std::to_string(m_planet->m_tech)),
		glm::vec3(-0.25f, 0.05f, 0) + pos(),
		true, true, glm::vec4(0.75f), 0.1f));

	m_planet->m_textRenderer->drawText(Text(
		std::string("Reso " + std::to_string(m_planet->m_resource)),
		glm::vec3(0.25f, 0.05f, 0) + pos(),
		true, true, glm::vec4(0.75f), 0.1f));

	// Render planet state
	std::string stateText = "Uncolonized";
	glm::vec4 stateColor(0.75f);
	
	switch (m_planet->state())
	{
	case Planet::State::Colonizing:
	{
		stateText = "Colonizing ";
		stateText += m_planet->m_colonizingEmpire->name();
		stateColor = glm::vec4(m_planet->m_color, stateColor.a);

		 m_planet->m_textRenderer->drawText(Text(
			"Progress",
			glm::vec3(0.75f, 0.05f, 0) + pos(),
			true, true, glm::vec4(m_planet->m_colonizingEmpire->color(), 0.75f), 0.1f));
		 m_planet->m_textRenderer->drawText(Text(
			std::to_string(static_cast<int>(m_planet->m_colonizingProgress * 100.f)) + " %",
			glm::vec3(0.75f, -0.05f, 0) + pos(),
			true, true, glm::vec4(m_planet->m_colonizingEmpire->color(), 0.75f), 0.1f));

		break;
	}
	case Planet::State::Influencing:
	{
		stateText = "Influencing ";
		stateText += m_planet->m_colonizingEmpire->name();
		stateColor = glm::vec4(m_planet->m_color, stateColor.a);

		// Duplicated code
		 m_planet->m_textRenderer->drawText(Text(
			"Progress",
			glm::vec3(0.75f, 0.05f, 0) + pos(),
			true, true, glm::vec4(m_planet->m_colonizingEmpire->color(), 0.75f), 0.1f));
		 m_planet->m_textRenderer->drawText(Text(
			std::to_string(static_cast<int>(m_planet->m_colonizingProgress * 100.f)) + " %",
			glm::vec3(0.75f, -0.05f, 0) + pos(),
			true, true, glm::vec4(m_planet->m_colonizingEmpire->color(), 0.75f), 0.1f));

		break;
	}

	case Planet::State::Colonized:
	{
		stateText = "Colonized ";
		stateText += m_planet->m_empire->name();
		stateColor = glm::vec4(m_planet->m_color, stateColor.a);

		if (!m_planet->isEnemyEmpire(*m_planet->m_empire))
		{
			 m_planet->m_textRenderer->drawText(Text(
				std::string("Credits " + std::to_string((int)m_planet->credits())),
				glm::vec3(-0.45f, -0.15f, 0) + pos(),
				false, true, glm::vec4(0.75f), 0.1f));
			 m_planet->m_textRenderer->drawText(Text(
				 std::string("Fighters " + std::to_string(m_planet->m_fighters.size()) + '/' +std::to_string(m_planet->m_resource)),
				 glm::vec3(-0.45f, -0.225f, 0) + pos(),
				 false, true, glm::vec4(0.75f), 0.1f));


			// Render planet build options
			for (const auto &button : m_buttons)
			{
				button->drawTransparent(camera);
			}
		}
	}
	break;

	case Planet::State::Influenced:
	{
		stateText = "Influenced ";
		stateText += m_planet->m_empire->name();
		stateColor = glm::vec4(m_planet->m_color, stateColor.a);

		if (!m_planet->isEnemyEmpire(*m_planet->m_empire))
		{
			// More duplicated code
			 m_planet->m_textRenderer->drawText(Text(
				std::string("Credits " + std::to_string((int)m_planet->credits())),
				glm::vec3(-0.45f, -0.15f, 0) + pos(),
				false, true, glm::vec4(0.75f), 0.1f));
		}
	}
	break;
	}

	 m_planet->m_textRenderer->drawText(Text(
		stateText,
		glm::vec3(-0.45f, -0.05f, 0) + pos(),
		false, true, stateColor, 0.1f));	
}