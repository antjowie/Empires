#include "Scene.h"

Scene::Scene():
	m_wantToPop(false)
{
}

Scene::~Scene()
{
}

void SceneContainer::pushInitialScene(Scene * scene)
{
	m_sceneStack.push(scene);
}

void SceneContainer::run(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime)
{
	Scene * scene = m_sceneStack.top()->run(drawableFactory, input, elapsedTime);
	
	if (m_sceneStack.top()->m_wantToPop)
	{
		delete m_sceneStack.top();
		m_sceneStack.pop();
	}

	if (scene)
	{
		m_sceneStack.push(scene);
		scene->onCreate(drawableFactory);
	}
}

bool SceneContainer::empty() const
{
	return m_sceneStack.empty();
}
