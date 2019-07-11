#pragma once
#include "Hitbox.h"

class SphereHitbox : public Hitbox
{
public:

	SphereHitbox(const Drawable & drawable);

	virtual float intersect(const Ray &ray) const override final;
	virtual bool contains(const glm::vec3 &point) const override final;
};