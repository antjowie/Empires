#pragma once
#include "Scene.h"
#include "drawables/HUDs/HUD.h"
#include "drawables/Planet.h"
#include "drawables/Skybox.h"
#include "TextRenderer.h"

#include <memory>

class HUDTestScene : public Scene
{
private:
	std::unique_ptr<Camera> m_camera;

	ClickableSelector m_clickableSelector;
	TextRenderer m_textRenderer;

	std::vector<std::unique_ptr<Vessel>> m_vessels;

	std::unique_ptr<Skybox> m_skybox;
	std::unique_ptr<HUD> m_ui;
	std::unique_ptr<Planet> m_planet;
public:

	virtual void onCreate(const DrawableFactory &drawableFactory) override final;
	virtual Scene *run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime) override final;
};