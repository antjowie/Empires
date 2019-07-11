#include "scenes/HUDTestScene.h"
#include "cameras/FreelookCamera.h"

void HUDTestScene::onCreate(const DrawableFactory & drawableFactory)
{
	m_textRenderer.init();
	m_textRenderer.loadFont("resources/fonts/roboto.ttf");

	m_camera = std::make_unique<FreelookCamera>(50.f);

	m_skybox = drawableFactory.createDrawable<Skybox>("skybox");
	m_skybox->setColor(glm::vec3(0, 0.25f, 0));

	m_ui = drawableFactory.createDrawable<HUD>("ui");
	m_ui->setClickableSelector(m_clickableSelector);
	//m_ui->setTextRenderer(m_textRenderer);
	m_ui->setPos(glm::vec3(0, -0.75f, -0.1f));
	m_ui->setScale(glm::vec3(1,0.25f,1));


	m_planet = drawableFactory.createDrawable<Planet>("planet");
	m_planet->setClickableSelector(m_clickableSelector);
	m_planet->setTextRenderer(m_textRenderer);
	m_planet->setPos(glm::vec3(0, 0, -5));
	m_planet->setTech(5);
	m_planet->setName("HUD Test");
}

Scene * HUDTestScene::run(const DrawableFactory & drawableFactory, const Input & input, float elapsedTime)
{
	m_camera->handleInput(input, elapsedTime);
	m_clickableSelector.updateIntersection(drawableFactory, input, elapsedTime, *m_camera);

	//m_vessels.push_back(m_planet->createVesselIfPossible(drawableFactory));
	if (!m_vessels.back())
		m_vessels.pop_back();

	m_planet->update(elapsedTime);
	//m_ui->updateColor();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	m_skybox->draw(*m_camera);
	m_planet->draw(*m_camera);
	
	for (const auto &vessel : m_vessels)
	{
		vessel->move(glm::vec3(elapsedTime));
		vessel->rotate(glm::vec3(elapsedTime * 10.f));
		vessel->draw(*m_camera);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_ui->draw(*m_camera);

	glDepthFunc(GL_ALWAYS);
	m_textRenderer.render(*m_camera);

	return nullptr;
}