#pragma once
#include "Scene.h"
#include "Drawable.h"
#include "Camera.h"
#include "drawables/Planet.h"

#include <memory>

class GameScene: public Scene
{
private:
	std::unique_ptr<Camera> m_camera;
	TextRenderer m_textRenderer;

	std::vector<std::unique_ptr<Planet>> m_planets; // Stupid limitation on my part. Factory returns unique ptr so that
													// I don't have to worry about allocating anything, but planets don't really
													// need to be pointers as they never have children (for this game)	

public:
	virtual void onCreate(const DrawableFactory &drawableFactory) override final;
	virtual Scene *run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime) override final;
};