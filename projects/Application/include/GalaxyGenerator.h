#pragma once
#include "drawables/Planet.h"
#include "prngs/PRNG.h"

#include "DrawableFactory.h"
#include "TextRenderer.h"
#include "LineRenderer.h"

#include <vector>

class GalaxyGenerator
{
public:
	float m_xMax;
	float m_zMax;

	void generate(
		std::vector<std::unique_ptr<Planet>> &planets,
		std::vector<std::unique_ptr<Vessel>> &vesselsToAdd,
		ClickableSelector &clickableSelector,
		TextRenderer &textRenderer,
		LineRenderer &lineRenderer,
		const DrawableFactory &factory, 
		std::unique_ptr<PRNG> prng);
};