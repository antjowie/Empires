#include "scenes/TSPScene.h"
#include "TSP.h"
#include "drawables/Planet.h"

#include <cassert>
#include <list>

void TSPScene::onCreate(const DrawableFactory & drawableFactory)
{
	std::vector<std::unique_ptr<Planet>> planets;

	static constexpr glm::vec2 vertices[5] = 
	{
		{0,0}, {20,5},
		{10,15},
		{5,30}, {35,20}
	};

	for (int i = 0; i < 5; i++)
	{
		planets.push_back(drawableFactory.createDrawable<Planet>("planet"));
		planets.back()->setPos(glm::vec3(vertices[i], 0));
	}

	std::vector<Planet*> toVisit;
	for (const auto &elem : planets)
		toVisit.push_back(elem.get());
	TSP tsp;
	tsp.createWeightMatrix(toVisit);

	tsp.m_verbose = true;
	toVisit = tsp.shortestPath(*planets.front().get(), *planets.front().get(), toVisit);

	std::cout << "Addresses ";
	for (const auto &elem : toVisit)
		std::cout << elem << ' ';
	std::cout << '\n';

}

Scene * TSPScene::run(const DrawableFactory & drawableFactory, const Input & input, float elapsedTime)
{
	m_wantToPop = true;

	return nullptr;
}
