#include "hitboxes/SphereHitbox.h"

SphereHitbox::SphereHitbox(const Drawable & drawable) :
	Hitbox(drawable)
{
}

float SphereHitbox::intersect(const Ray & ray) const
{
	// These are the values used by my formula
	const glm::vec3 e = ray.origin();
	const glm::vec3 d = ray.direction();
	const glm::vec3 c = m_drawable.pos();

	// These are the values used by the quadratic expression
	const float A = glm::dot(d,d);
	const float B = 2.f * glm::dot(e-c,d);

	float largestScale = 0;
	//largestScale = glm::max(largestScale, m_drawable.scale().x);
	//largestScale = glm::max(largestScale, m_drawable.scale().y);
	//largestScale = glm::max(largestScale, m_drawable.scale().z);
	largestScale = m_drawable.scale().x;

	const float C = glm::dot(e,e) + glm::dot(c,c) - (2.f * glm::dot(e,c)) - (largestScale * largestScale);

	float discriminant = B * B - 4.f * A*C;

	if (discriminant < 0.f)
		return -1.f;

	discriminant = glm::sqrt(discriminant);
	float t1 = (-B + discriminant) / (2.f*A);
	float t2 = (-B - discriminant) / (2.f*A);

	if (t1 < 0.f)
		t1 = t2;
	if (t2 < 0.f)
		t2 = t1;

	return t1 < t2 ? t1 : t2;
}

bool SphereHitbox::contains(const glm::vec3 & point) const
{
	const float distance = glm::distance(point, m_drawable.pos());
	return (distance <= m_drawable.scale().x);
}
