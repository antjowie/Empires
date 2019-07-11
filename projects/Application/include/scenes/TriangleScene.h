#pragma once
#include "Scene.h"
#undef countof
#include "glm/glm/vec3.hpp"


class TriangleScene : public Scene
{
private:
	GLuint m_vbo;
	GLuint m_ebo;
	glm::vec3 m_rotations;
	float m_time;

public:
	virtual void onCreate(const DrawableFactory &drawableFactory) override final;
	virtual Scene* run(const DrawableFactory &factory, float elapsedTime) override final;
};