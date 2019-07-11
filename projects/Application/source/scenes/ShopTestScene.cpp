#include "scenes/ShopTestScene.h"
#include "cameras/FreelookCamera.h"
#include "GalaxyGenerator.h"
#include "prngs/xorshf96.h"

void ShopTestScene::onCreate(const DrawableFactory & drawableFactory)
{
	m_textRenderer.init();
	m_textRenderer.loadFont("resources/fonts/roboto.ttf");
	
	//GalaxyGenerator::generate(
	//	m_planets,
	//	m_vessels, 
	//	m_clickableSelector, 
	//	m_textRenderer, 
	//	drawableFactory, 
	//	std::make_unique<Xorshf96>(0xDEADBEEF));

	m_camera = std::make_unique<FreelookCamera>(50.f);
	m_camera->setNearFar(glm::vec2(0.1f, 1000.f));

	m_cursor = drawableFactory.createDrawable<Cursor>("cursor");
	m_cursor->setScale(0.025f);

	m_skybox = drawableFactory.createDrawable<Skybox>("skybox");
	m_skybox->setColor(glm::vec4(0, 0, 0, 1));
}

Scene * ShopTestScene::run(const DrawableFactory & drawableFactory, const Input & input, float elapsedTime)
{
	m_camera->handleInput(input, elapsedTime);

	m_clickableSelector.updateIntersection(drawableFactory, input, elapsedTime, *m_camera);

	int i = 0;
	for (std::vector<std::unique_ptr<Planet>>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++)
	{
		if (i++ % 8 != 0)
			(*planet)->m_clickable = false;
		else
		{
			(*planet)->update(elapsedTime);
			(*planet)->draw(*m_camera);
		}
	}

	// Because vessels and planets are not in the same data structure, I don't have to worry about moving over stuff from the vectors
	for (std::vector<std::unique_ptr<Vessel>>::iterator vessel = m_vessels.begin(); vessel != m_vessels.end(); vessel++)
	{
		(*vessel)->move(glm::vec3(elapsedTime));
		(*vessel)->rotate(glm::vec3(elapsedTime * 20.f,elapsedTime * 5.f,elapsedTime * 10.f));
		(*vessel)->draw(*m_camera);
	}

	m_textRenderer.render(*m_camera);
	
	m_cursor->setPos(glm::vec3(input.GetMousePos(), -0.1f));
	m_cursor->draw(*m_camera);

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE); 
	// The issue is basically that we render to a surface. When we have transparent values in our buffer
	// we can see the desktop through it. To fix this, I just make every value in the color buffer opaque.
	// This can be quite expensive because I loop over all fragments but it does solve the problem. 
	//m_skybox->setPos(m_camera->pos());
	m_skybox->draw(*m_camera);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return nullptr;
}