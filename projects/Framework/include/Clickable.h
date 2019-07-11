#pragma once
#include "Hitbox.h"
#include "DrawableFactory.h"

#include <memory>
#include <map>
#include <stack>

class ClickableSelector;

class Clickable
{
	friend ClickableSelector; // Used for access to key
private:
	std::unique_ptr<Hitbox> m_hitbox;

	unsigned m_key;
	ClickableSelector * m_clickableSelector;

protected:
	ClickableSelector * clickableSelector() const;

public:
	// This is needed by vessel because it needs to know if the user has clicked on a planet.
	// This also kinda ruins the whole interface of clickable. A better solution would be to 
	// add a function that overwrites the clickableManager and allows the user to check for a
	// intersection with any special type of object. But this would require quite a bit of 
	// writing and I think that it is not needed for this project
	enum class Type
	{
		vessel,
		planet,
		other // Yea same as a don't care type
	};
	const Type m_type;
	
	Clickable(Hitbox *hitbox, const bool clickable = true, Type type = Type::other);
	virtual ~Clickable();

	bool m_clickable;
	void setClickableSelector(ClickableSelector & clickableSelector);
	const Hitbox &hitbox() const;

	virtual void onUnselect(Clickable *clickable);
	virtual void onClick(const Ray &ray);
	virtual void onHover(const Ray &ray);
};

class ClickableSelector
{
private:
	std::map<unsigned, Clickable*> m_clickables;
	std::stack<unsigned> m_freeIndices;

	Clickable *m_lastClicked;

	unsigned m_currentKey;
	float m_clickCooldown;

public:
	ClickableSelector();

	void addClickable(Clickable *clickable);
	void removeClickable(unsigned key);

	void updateIntersection(const DrawableFactory &drawableFactory, const Input &input, float elapsedTime, const Camera &camera);
};