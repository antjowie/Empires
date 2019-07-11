#include "hitboxes/HUDHitbox.h"

HUDHitbox::HUDHitbox(const Drawable & drawable) :
	Hitbox(drawable)
{
}

float HUDHitbox::intersect(const Ray & ray) const
{
	const glm::vec2 size(m_drawable.scale());


	// Orthogonal projection
	const glm::vec2 point(ray.mousePos() - glm::vec2(m_drawable.pos()));

	if (point.x > -size.x && point.x < size.x &&
		point.y > -size.y && point.y < size.y)
		return -m_drawable.pos().y;
	return -1.f;
	//const float p1 = (center[0] - size[0] - rayOrigin[0]) / rayDirection[0];
	//const float p2 = (center[0] + size[0] - rayOrigin[0]) / rayDirection[0];
	//
	//float tMin = std::fminf(p1, p2);
	//float tMax = std::fmaxf(p1, p2);
	//
	//for (int i = 1; i < 2; i++)
	//{
	//	if (rayDirection[i] != 0.f)
	//	{
	//		const float pos1 = (center[i] - size[i] - rayOrigin[i]) / rayDirection[i];
	//		const float pos2 = (center[i] + size[i] - rayOrigin[i]) / rayDirection[i];
	//
	//		// If we look at the box from the back
	//		// pos2 can be the min and pos1 the max
	//		tMin = std::fmaxf(tMin, std::fminf(pos1, pos2));
	//		tMax = std::fminf(tMax, std::fmaxf(pos1, pos2));
	//	}
	//}
	//
	//if (tMin >= tMax)
	//	return -1.f;
	//
	//if (tMin < 0.f && tMax > 0.f)
	//	return tMax;
	//return tMin;
}