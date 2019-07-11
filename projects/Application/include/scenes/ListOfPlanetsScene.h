#pragma once
#include "Scene.h"

class ListOfPlanetsScene : public Scene
{
private:

public:
	ListOfPlanetsScene();

	virtual void onCreate(const DrawableFactory &drawableFactory) override final;
	virtual Scene *run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime) override final;
};