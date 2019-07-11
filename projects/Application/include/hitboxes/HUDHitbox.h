#pragma once
#include "Hitbox.h"

class HUDHitbox : public Hitbox
{
public:
	HUDHitbox(const Drawable & drawable);

	virtual float intersect(const Ray &ray) const override final;
};