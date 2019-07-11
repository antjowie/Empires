#pragma once
#include "Drawable.h"

class Ray
{
private:
	glm::vec3 m_origin;
	glm::vec3 m_direction;
	glm::vec2 m_mousePos;

public:
	Ray(const glm::vec3 &origin, const glm::vec3 &direction, const glm::vec2 &mousePos);

	const glm::vec3 &origin() const;
	const glm::vec3 &direction() const;
	const glm::vec2 &mousePos() const;
};

class Hitbox
{
protected:
	const Drawable & m_drawable;

public:

	Hitbox(const Drawable &drawable);

	//	virtual bool contains(const glm::vec3 &point) = 0;
	virtual float intersect(const Ray &ray) const = 0;
	virtual bool contains(const glm::vec3 &point) const;
};