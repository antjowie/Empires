#include "Planet.h"
#include "prngs/xorshf96.h"
#include "scenes/ListOfPlanetsScene.h"
#include "GalaxyGenerator.h"

ListOfPlanetsScene::ListOfPlanetsScene() :
	Scene()
{
}

void ListOfPlanetsScene::onCreate(const DrawableFactory &drawableFactory)
{
	std::vector<Planet> m_planets;
	GalaxyGenerator::generate(m_planets, std::make_unique<Xorshf96>(0xDEADBEEF));
	int i = 1;
	for (const Planet &planet : m_planets)
	{
		std::printf("Planet #%i: %s tech: %i resource: %i\n", i++, planet.name().c_str(), planet.tech(), planet.resource());
	}
}

Scene * ListOfPlanetsScene::run(const DrawableFactory & drawableFactory, const Input & input, float elapsedTime)
{
	m_wantToPop = true;
	return nullptr;
}