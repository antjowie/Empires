#pragma once
#include "Drawable.h"

class Skybox : public Drawable
{
private:
	glm::vec4 m_color;

public:
	Skybox(const DrawData &drawdata);
	void setColor(const glm::vec3 &color);

	virtual void draw(const Camera &camera) const override final;
};