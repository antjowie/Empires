#include "scenes/ClickableTestScene.h"
#include "cameras/FreelookCamera.h"

void ClickableTestScene::onCreate(const DrawableFactory & drawableFactory)
{
	m_textRenderer.init();
	m_textRenderer.loadFont("resources/fonts/roboto.ttf");

	m_camera = std::make_unique<FreelookCamera>(150.f);

	for (float i = 1; i < 10.f; i++)
	{
		m_planets.push_back(drawableFactory.createDrawable<Planet>("planet"));
		Planet * planet = m_planets.back().get();
		planet->setClickableSelector(m_clickableSelector);
		planet->setTextRenderer(m_textRenderer);
		planet->setPos(glm::vec3(i * (3.f + i / 5.f), 0, -5.f));
		std::cout << planet->pos().x << '\n';
		planet->setName(std::string("Planet # ") + std::to_string((unsigned)i));
		planet->setTech((unsigned)i);
	}
}

Scene * ClickableTestScene::run(const DrawableFactory & drawableFactory, const Input & input, float elapsedTime)
{
	m_camera->handleInput(input, elapsedTime);

	m_clickableSelector.updateIntersection(drawableFactory, input, elapsedTime, *m_camera);
	
	for (auto planet = m_planets.begin(); planet != m_planets.end(); planet++)
	{
		(*planet)->update(elapsedTime);
		(*planet)->draw(*m_camera);
	}

	m_textRenderer.render(*m_camera);

	return nullptr;
}