#pragma once
#include "DrawableFactory.h"
#include "Input.h"
#include "Camera.h"

#include <stack>

class Scene
{
public:
	Scene();
	bool m_wantToPop;

	virtual void onCreate(const DrawableFactory &drawableFactory) = 0;
	virtual Scene *run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime) = 0;

	virtual ~Scene();
};

class SceneContainer
{
private:
	std::stack<Scene*> m_sceneStack;

public:
	void pushInitialScene(Scene * scene);
	
	void run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime);

	bool empty() const;

};
