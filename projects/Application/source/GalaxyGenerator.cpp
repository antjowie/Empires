#include "GalaxyGenerator.h"
#include "Timer.h"

#include <cmath>
#include <algorithm>
#include <map>

void GalaxyGenerator::generate(
	std::vector<std::unique_ptr<Planet>> &planets, 
	std::vector<std::unique_ptr<Vessel>> &vesselsToAdd, 
	ClickableSelector & clickableSelector, 
	TextRenderer & textRenderer,
	LineRenderer &lineRenderer,
	const DrawableFactory &factory, 
	std::unique_ptr<PRNG> prng)
{
	Timer timer;
	std::cout << "Generating galaxy...\n";

	// Taken from and added a bit of my own
	// http://blog.rabidgremlin.com/2015/01/14/procedural-content-generation-creating-a-universe/
	constexpr static unsigned char pairs[] =
		"..a.algeuyooare."
		"uri.may.atenuioa"
		"erewo.isoau.orow"
		"edou..leonaiveti"
		"..a.alisoageuye."
		"eee.xeou..leonre"
		"atenuri.may.uioa"
		"eee.xeeieaayeyre"

		"usu.maesxeindioi"
		"laquanueenedbera"
		"usesu.maindixeoi"
		"erewo.ooaru.orow"
		"bezarausbiueenar"
		"bezaceusesi.biar"
		"edaieieaayeyveti"
		"laquanesi.edbece";

	std::map<int, int> map;
	std::map<std::string, int> names;
	std::map<int, int> seeds;
	
	// 128 * 64 = 8192
	// Can't find nice decimal for x and y so it may change later on
	// I'm using floats cus I don't feel like casting in the code. To my suprise the ++ operator works
	//	for(float x = 0.f; x < 128.f; x++)
	//		for (float z = 0.f; z > -64.f; z--)
	
	//constexpr float xMax = 11.f * 12.f;
	//constexpr float zMax = -7.f * 12.f;

	for (float x = 0.f; x < m_xMax; x++)
	{
		for (float z = 0.f; z > m_zMax; z--)
		{
			// Create elliptical galaxy
			const float rx = m_xMax * 0.5f;
			const float rz = m_zMax * 0.5f;

			const float length = (pow(x - rx,2)/ (rx*rx) + pow(z - rz, 2)/(rz*rz));
			if (length > 1.f)
				continue;

			const uint32_t seed = prng->random();
	
			// Select random pairs from array
			// Mask = 0011 1111
			const uint8_t pair1 = static_cast<uint8_t>(2 * ((seed >> 8) & 127));
			const uint8_t pair2 = static_cast<uint8_t>(2 * ((seed >> 16) & 127));
			const uint8_t pair3 = static_cast<uint8_t>(2 * ((seed >> 24) & 127));
			const uint8_t pair4 = static_cast<uint8_t>(2 * ((seed ^ (seed >> 7)) & 127));
			const uint8_t pair5 = static_cast<uint8_t>(2 * ((seed ^ (seed >> 15)) & 127));
			const uint8_t pair6 = static_cast<uint8_t>(2 * ((seed ^ (seed >> 23)) & 127));
	
			// Randomize the order in which pairs are added to the name
			unsigned char iter = seed & 3;
			std::string name;
			for (int i = 0; i < 4; i++)
			{
				// Add a space to the planet name
				if (i == 3 && (seed & 1))
				{
					name += ' ';
					//static_cast<char>(toupper(name[0]))
					name += static_cast<char>(toupper(pairs[pair6]));
					name += pairs[pair6 + 1];
				}
	
				switch (iter)
				{
				case 0:
					name += pairs[pair1];
					name += pairs[pair1 + 1];
					break;
				case 1:
					name += pairs[pair2];
					name += pairs[pair2 + 1];
					break;
				case 2:
					name += pairs[pair3];
					name += pairs[pair3 + 1];
					break;
				case 3:
					name += pairs[pair4];
					name += pairs[pair4 + 1];
					break;
				}
				if (++iter > 3)
					iter = 0;
			}
			// Extend the planet name
			if (seed & 0x10)
			{
				name += pairs[pair5];
				name += pairs[pair5 + 1];
			}
	
			// Remove all dots in the string
			name.erase(std::remove(std::begin(name), std::end(name), '.'), std::end(name));
			name[0] = static_cast<char>(toupper(name[0]));
	
			// Calculate tech and resource level
			constexpr float power = 1.75f;
			unsigned tech = seed & 0xFF;
			tech = unsigned(std::pow(static_cast<float>(tech) / (float)0xFF, power) * 14.5f + 1.f);
	
			unsigned resource = ((seed >> 8) & 0xFF);
			resource = unsigned(std::pow(static_cast<float>(resource) / (float)0xFF, power) * 14.5f + 1.f);
	
			// Calculate position of planet
			// Temp make planet 4 times as big as a ship
			//const glm::vec3 pos(
			//	x * 20.f + float(prng->random() % 100) / 10.f - 5.f,
			//	float(prng->random() % 100) / 10.f - 5.f,
			//	z * 20.f + float(prng->random() % 100) / 10.f - 5.f);

			// 255 -> 25.5
			const glm::vec3 pos(
				x * 40.f + float((seed >> 0 ) & 0xFF) * 0.1f - 12.25f,
						   float((seed >> 8 ) & 0xFF) * 0.1f - 12.25f,
				z * 40.f + float((seed >> 16) & 0xFF) * 0.1f - 12.25f);

			planets.push_back(factory.createDrawable<Planet>("planet"));
			Planet *planet = static_cast<Planet*>(planets.back().get());

			// Set variables of planets
			//planet->setPos(pos);
			planet->setClickableSelector(clickableSelector);
			planet->init(pos, name, tech, resource, lineRenderer, textRenderer, factory,vesselsToAdd);
			
			planet->setPlanetsVector(planets);

			// Do some tracking of variables
			if (map.count(tech) == 0)
				map[tech] = 0;
			map[tech]++;
	
			if (names.count(name) == 0)
				names[name] = 0;
			names[name]++;
			
			if (seeds.count(seed) == 0)
				seeds[seed] = 0;
			seeds[seed]++;
		}
	}

	// NOTE: this takes too long so I calculate it when a planet is no longer colonized

	//std::cout << "Calculating planet SOI intersections...\n";
	//Timer soiTimer;
	//for (size_t i = 0; i < planets.size(); i++)
	//{
	//	planets[i]->fillPlanetsInSOI(planets);
	//	if (soiTimer.elapsedTime() > 5.f)
	//	{
	//		soiTimer.restart();
	//		std::printf("%i from the %i (%.2f%%)\n", i, planets.size(), float(i) / float(planets.size()) * 100.f);
	//	}
	//}

	//for (const std::unique_ptr<Planet> &planet : planets)
	//	planet->fillPlanetsInSOI(planets);

	// Debug stuff
	// -----------------------------------------------

	std::printf("---\nGALAXY RESULTS\n---\nGenerated %i planets\n", planets.size());
	// Check tech level generation stats
	for (auto i : map)
		std::printf("tech level %i occured %i times\n", i.first, i.second);

	// Check for duplicate names
	for (auto i : names)
		if(i.second > 1)
			std::printf("NAME: %s occured %i times\n", i.first.c_str(), i.second);
	std::printf("%i unique names\n", names.size());

	// Check for duplicate seeds
	bool duplicateSeed = false;
	for (auto i : seeds)
		if (i.second > 1)
		{
			std::printf("SEED: %i occured %i times\n", i.first, i.second);
			duplicateSeed = true;
		}
	if (!duplicateSeed)
		std::cout << "All planets used an unique seed\n";

	std::printf("It took %f seconds to generate the galaxy\n", timer.elapsedTime());
}