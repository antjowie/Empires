#pragma once
#include "Scene.h"
#include "TextRenderer.h"

#include "Camera.h"
#include <memory>

class MainMenuScene : public Scene
{
private:
	TextRenderer m_textRenderer;

	std::unique_ptr<Camera> m_camera;
	float m_totalTime;

public:
	virtual void onCreate(const DrawableFactory &drawableFactory) override final;
	virtual Scene *run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime) override final;
};