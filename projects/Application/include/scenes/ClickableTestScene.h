#pragma once
#include "Scene.h"
#include "drawables/Planet.h"
#include "TextRenderer.h"

#include <vector>

class ClickableTestScene : public Scene
{
private:
	std::unique_ptr<Camera> m_camera;

	ClickableSelector m_clickableSelector;

	TextRenderer m_textRenderer;
	std::vector<std::unique_ptr<Planet>> m_planets;

public:
	virtual void onCreate(const DrawableFactory &drawableFactory) override final;
	virtual Scene *run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime) override final;
};