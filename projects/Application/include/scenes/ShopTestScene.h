#pragma once
#include "Scene.h"
#include "TextRenderer.h"
#include "drawables/Planet.h"
#include "drawables/HUDs/Cursor.h"
#include "drawables/Skybox.h"

#include <vector>

class ShopTestScene: public Scene
{
private:
	std::unique_ptr<Camera> m_camera;

	ClickableSelector m_clickableSelector;
	TextRenderer m_textRenderer;

	std::vector<std::unique_ptr<Planet>> m_planets;
	//std::unique_ptr<Planet> m_planet;
	std::vector<std::unique_ptr<Vessel>> m_vessels;

	std::unique_ptr<Cursor> m_cursor;

	std::unique_ptr<Skybox> m_skybox;

public:
	virtual void onCreate(const DrawableFactory &drawableFactory) override final;
	virtual Scene *run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime) override final;
};