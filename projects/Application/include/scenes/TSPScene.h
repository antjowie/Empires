#pragma once
#include "Scene.h"

class TSPScene : public Scene
{
public:
	virtual void onCreate(const DrawableFactory &drawableFactory) override final;
	virtual Scene *run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime) override final;
};