#include "scenes/PlaygroundScene.h"
#include "cameras/FreelookCamera.h"
#include "GalaxyGenerator.h"
#include "prngs/xorshf96.h"

#include <algorithm>

void PlaygroundScene::onCreate(const DrawableFactory & drawableFactory)
{
	Timer timer;
	m_fullscreen = false;
	m_fullscreenCooldown = 0.f;

	m_lineRenderer.init(400,1.f);
	
	m_textRenderer.init();
	m_textRenderer.loadFont("resources/fonts/roboto.ttf");

	GalaxyGenerator generator;
	generator.m_xMax = 11.f * 12.f;
	generator.m_zMax = -7.f * 12.f;
	//constexpr float xMax = 11.f * 12.f;
	//constexpr float zMax = -7.f * 12.f;

	generator.generate(
		m_planets,
		m_vessels,
		m_clickableSelector,
		m_textRenderer,
		m_lineRenderer,
		drawableFactory,
		std::make_unique<Xorshf96>(0xDEADBEEF));
	
	// Comment this out if you are prototyping, it takes a long time to boot the game but
	// saves on lag during runtime
	//Timer t2;
	//std::cout << "Calculating planets contained in SOIs...\n";
	//for (size_t i = 0; i < m_planets.size(); i++)
	//{
	//	m_planets[i]->fillPlanetsInSOI();
	//	if (t2.elapsedTime() > 3.f)
	//	{
	//		t2.restart();
	//		std::printf("%i/%i (%.2f) elapsed: %.2f seconds\n", i, m_planets.size(), float(i) / float(m_planets.size()) * 100.f, t2.totalTime());
	//	}
	//}
	//std::cout << "Finished filling SOIs in " << t2.totalTime() << " seconds\n";

	m_camera = std::make_unique<FreelookCamera>(50.f, 25.f);
	m_camera->setNearFar(glm::vec2(0.1f, 10000.f));
	m_camera->setPos(glm::vec3(m_planets.back()->pos().x * 0.5f, 50, -9 * 12));
	//m_camera->setTarget(glm::vec3(96, 20, -60));

	m_cursor = drawableFactory.createDrawable<Cursor>("cursor");
	m_cursor->setScale(0.025f);

	m_skybox = drawableFactory.createDrawable<Skybox>("skybox");
	m_skybox->setColor(glm::vec4(0, 0, 0, 1));

	m_empire.setColor(glm::vec3(0.25f, 1.f, 0.25f));
	m_empire.setName("Good dictator");
	m_empire.setHomePlanet(*m_planets[0].get());
	m_planets.front()->setEmpire(m_empire);
	m_empire.markAsPlayer();

	m_empire2.setColor(glm::vec3(1.f, 0.25f, 0.25f));
	m_empire2.setName("Bad dictator");
	m_empire2.setHomePlanet(*m_planets.back());
	m_planets.back()->setEmpire(m_empire2);

	m_map = drawableFactory.createDrawable<Map>("map");
	m_map->setClickableSelector(m_clickableSelector);

	//m_map->setLevelBounds(glm::vec3(16 * 12, 9 * 12, 1));
	m_map->setLevelBounds(glm::vec3(generator.m_xMax, -generator.m_zMax, 1));
	m_map->setViewportHeight(800);
	
	m_map->generateMap(m_planets);
	m_map->generateMarkers(4); // 4 represents 4 empires although this should be seen as a capacity (like vector)

	m_planetRenderer.fillPlanetsVBO(m_planets);
	m_camera->setPos(m_empire.homePlanet()->pos() + glm::vec3(0, 0, 10.f));

	std::printf("Initialized scene in %f seconds\n", timer.elapsedTime());
}

Scene * PlaygroundScene::run(const DrawableFactory & drawableFactory, const Input & input, float elapsedTime)
{
	// Input phase
	// ------------------
	if (input.Keys[KEY_ESC])
		m_wantToPop = true;

	if (input.Keys[KEY_M] && m_fullscreenCooldown == 0.f)
	{
		m_fullscreen = !m_fullscreen;

		m_map->setFullscreen(m_fullscreen);
		m_fullscreenCooldown = 1.f;

		m_map->updatePlanetsSOI(m_planets);
	}
	m_fullscreenCooldown -= elapsedTime;
	if (m_fullscreenCooldown < 0.f)
		m_fullscreenCooldown = 0.f;

	// Update map zoom
	if (input.Keys[KEY_MINUS])
	{
		m_map->zoom(1.f - 0.5f * elapsedTime);
		m_map->updatePlanetsSOI(m_planets);
	}
	if (input.Keys[KEY_EQUAL])
	{
		m_map->zoom(1.f + 0.5f * elapsedTime);
		m_map->updatePlanetsSOI(m_planets);
	}
	if (input.Keys[KEY_BACKSPACE])
	{
		m_map->setZoom(1);
		m_map->updatePlanetsSOI(m_planets);
	}

	// Update fase
	// -----------------
	m_camera->handleInput(input, elapsedTime);

	//elapsedTime *= 500.f;

	m_clickableSelector.updateIntersection(drawableFactory, input, elapsedTime, *m_camera);

	if (m_updateVBOs.elapsedTime() > 2.5f)
	{
		m_map->updatePlanetsSOI(m_planets);
		m_planetRenderer.fillPlanetsVBO(m_planets);
		m_updateVBOs.restart();
	}

	m_map->setFocus(glm::vec2(m_camera->pos().x,m_camera->pos().z));

	// Draw/semi update fase
	// -----------------
	glDisable(GL_BLEND);

	m_skybox->draw(*m_camera);

	for (std::vector<std::unique_ptr<Planet>>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++)
	{
		(*planet)->update(elapsedTime);
	}

	for (std::vector<std::unique_ptr<Vessel>>::iterator vessel = m_vessels.begin(); vessel != m_vessels.end(); vessel++)
	{
		(*vessel)->update(elapsedTime);
	}

	// Because vessels and planets are not in the same data structure, I don't have to worry about moving over stuff from the vectors	
	// Brian's object files don't work
	// or I don't understand OpenGL
	if (!m_vessels.empty() && !m_vesselsToRemove.empty())
	{
		m_vessels.erase(std::remove_if(m_vessels.begin(), m_vessels.end(), [this](const std::unique_ptr<Vessel> &vessel)
		{
			for (Vessel * address : m_vesselsToRemove)
				if (vessel.get() == address)
					return true;
			return false;
		}));
		m_vesselsToRemove.clear();
	}

	glDisable(GL_CULL_FACE);
	for (std::vector<std::unique_ptr<Vessel>>::iterator vessel = m_vessels.begin(); vessel != m_vessels.end(); vessel++)
	{
		if ((*vessel)->isDead()) 
			m_vesselsToRemove.push_back(&(**vessel));
		(*vessel)->draw(*m_camera);
	}
	glEnable(GL_CULL_FACE);
	
	// Don't render all the other stuff if fullscreen map
	glEnable(GL_BLEND);
	
	m_map->addMarker(m_map->createMarker(m_camera->pos(), glm::vec4(m_empire.color(), 1.f), 1));
	if (m_fullscreen)
	{
		m_map->displayMarkers();
		m_map->drawTransparent(*m_camera);
		m_lineRenderer.display(*m_camera);
	}
	else
	{
		m_planetRenderer.display(*m_camera);
		m_empire.emitLine(m_lineRenderer);
		m_empire2.emitLine(m_lineRenderer);
		m_lineRenderer.display(*m_camera);

		for (std::vector<std::unique_ptr<Planet>>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++)
			(*planet)->drawTransparent(*m_camera);
		for (std::vector<std::unique_ptr<Vessel>>::iterator vessel = m_vessels.begin(); vessel != m_vessels.end(); vessel++)
			(*vessel)->drawTransparent(*m_camera);

		m_textRenderer.render(*m_camera);

		m_map->displayMarkers();
		m_map->drawTransparent(*m_camera); 

		m_cursor->setPos(glm::vec3(input.GetMousePos(), -0.1f));
		m_cursor->drawTransparent(*m_camera);
	}

	return nullptr;
}