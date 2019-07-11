#include "scenes/GameScene.h"
#include "GalaxyGenerator.h"

#include "prngs/xorshf96.h"
#include "cameras/FreelookCamera.h"

void GameScene::onCreate(const DrawableFactory &drawableFactory)
{
	m_camera = std::make_unique<FreelookCamera>(100.f);
	m_camera->setNearFar(glm::vec2(0.1f, 100.f));

	m_textRenderer.init();
	m_textRenderer.loadFont("resources/fonts/roboto.ttf");

	std::unique_ptr<ClickableSelector> temp = std::make_unique<ClickableSelector>();
	std::vector<std::unique_ptr<Vessel> >temp2;
	//GalaxyGenerator::generate(m_planets, temp2,*temp, m_textRenderer, , drawableFactory, std::make_unique<Xorshf96>(0xDEADBEEF));
}

Scene * GameScene::run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime)
{
	if (input.Keys[KEY_ESC])
		m_wantToPop = true;

	m_camera->handleInput(input, elapsedTime);

	// Update planets
	int i = 0;
	for (const std::unique_ptr<Planet> &planet: m_planets)
	{
		planet->update(elapsedTime);
		if (i++ % 8 == 0)
		planet->draw(*m_camera);
	}

	m_textRenderer.render(*m_camera);
	return nullptr;
}