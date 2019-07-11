#pragma once

#include "Window.h"
#include "Scene.h"
#include "DrawableFactory.h"
#include "Input.h"

class Game
{
private:
	using ObjectsToLoad = void (*)(DrawableFactory &);

	Window m_window;
	SceneContainer m_scenes;
	Input m_input;
	DrawableFactory m_factory;

public:
	bool init(unsigned width, unsigned height, Scene * initialScene, ObjectsToLoad objectsToLoad);
	
	void run();
};
