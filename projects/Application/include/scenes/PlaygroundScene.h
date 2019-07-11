#pragma once
#include "Scene.h"
#include "TextRenderer.h"
#include "LineRenderer.h"
#include "PlanetRenderer.h"

#include "Empire.h"
#include "Timer.h"

#include "drawables/Planet.h"
#include "drawables/HUDs/Cursor.h"
#include "drawables/Skybox.h"
#include "drawables/HUDs/Map.h"

#include <vector>

class PlaygroundScene : public Scene
{
private:
	TextRenderer m_textRenderer;
	LineRenderer m_lineRenderer;
	ClickableSelector m_clickableSelector;
	PlanetRenderer m_planetRenderer;

	std::unique_ptr<Camera> m_camera;

	std::vector<std::unique_ptr<Planet>> m_planets;
	std::vector<std::unique_ptr<Vessel>> m_vessels;
	std::vector<Vessel*> m_vesselsToRemove;

	std::unique_ptr<Cursor> m_cursor;
	std::unique_ptr<Skybox> m_skybox;
	std::unique_ptr<Map> m_map;

	Empire m_empire;
	Empire m_empire2;

	// Related to map, maybe should be inside map
	bool m_fullscreen;
	float m_fullscreenCooldown;
	Timer m_updateVBOs;

public:
	virtual void onCreate(const DrawableFactory &drawableFactory) override final;
	virtual Scene *run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime) override final;
};