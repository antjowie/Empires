#pragma once
#include "Drawable.h"
#include "Clickable.h"

#include "TextRenderer.h"
#include "LineRenderer.h"

#include "Empire.h"

#include "drawables/HUDs/ShopHUD.h"
#include "hitboxes/SphereHitbox.h"

#include <string>

class SOI : public Drawable
{
private:
	glm::vec4 m_color;
	SphereHitbox m_hitbox;

public:
	SOI(const DrawData &drawData);
	void init(const glm::vec3 &pos, float planetScale, const glm::vec3 &color = glm::vec3(0.1f));

	void setColor(const glm::vec3 &color); 
	const glm::vec4 &color() const;
	
	bool contains(const glm::vec3 &point);

	virtual void draw(const Camera &camera) const override final;
	virtual void drawTransparent(const Camera &camera) const override final;
};

class Planet : public Drawable , public Clickable
{
	friend ShopHUD;

public:
	enum class State
	{
		Colonized,
		Colonizing,
		Uncolonized,
		Influenced,
		Influencing,
		Siege
	};

private:
	// This is used to calculate the planets inside of the soi when
	// a planet gets colonized, cuz else I have to wait ~90 seconds to generate
	// a galaxy. Time complexity is n^2
	// Optimzation related
	std::vector<Planet*> m_planetsInSOI;
	const std::vector<std::unique_ptr<Planet>> *m_planets;
	bool m_planetsLoaded;
	float m_planetCheckTime;
	bool m_shouldInfluence;

	// Stats
	State m_state;
	std::string m_name;
	unsigned m_tech;
	unsigned m_resource;
	float m_credits;

	float m_colonizingProgress;
	Empire *m_colonizingEmpire;
	Empire *m_empire;

	// Fighter stuff
	float m_fighterCooldown;
	std::vector<FighterVessel*> m_fighters;
	std::vector<std::unique_ptr<Vessel>> *m_vesselsToAdd;

	// Flags
	bool m_hover;
	bool m_drawName;
	
	TextRenderer * m_textRenderer;
	LineRenderer * m_lineRenderer;
	const DrawableFactory * m_factory;

	// UI and additional drawables
	std::unique_ptr<ShopHUD> m_shopHUD;
	std::unique_ptr<SOI> m_soi;

	// End of variables
	// -----------------------

	void updateColonizationProgress(float elapsedTime, Empire &colonizingEmpire, State stateWhenComplete, float modifier);
	
public:
	Planet(const DrawData &drawdata);

	// Add to clickable selector first!!!
	void init(
		const glm::vec3 &position, 
		const std::string &name, 
		unsigned tech, 
		unsigned resource, 
		LineRenderer &lineRenderer,
		TextRenderer &textRenderer,
		const DrawableFactory &drawableFactory,
		std::vector<std::unique_ptr<Vessel>> &vesselsToAdd);

	// Call this after the tech level has been set because that decides
	// the size of the SOI
	// NOTE: This causes lag when updating the game too fast
	// Call this before the game starts results in quite a long initialization time
	void fillPlanetsInSOI();

	// When planet is colonized, it checks intersections. This makes sure that it doesn't take 90 seconds to boot game;
	void setPlanetsVector(const std::vector<std::unique_ptr<Planet>> &planets);

	virtual void update(float elapsedTime);

	void setHudClickable(bool clickable);
	void setEmpire(Empire & empire);

	void influence(float elapsedTime, Empire &influencingEmpire);
	void colonize(float elapsedTime, Empire &colonizingEmpire);
		
	// Planets and vessels need to remove themselves from the empire
	// this actually makes no sense because planet and vessel should not
	// have to know anything bout empire, but it was the fastest solution
	bool isEnemyEmpire(Empire &empire) const;
	Empire *colonizingEmpire() const;
	float credits() const;
	const glm::vec3 &color() const;
	SOI * soi() const;

	State state() const;
	const std::vector<Planet*> &planetsInSOI() const;
	// Returns distance
	float boundsDistance(const glm::vec3 &pos, float scale) const;

	// Used for carrier vessel
	std::vector<FighterVessel*> requestFighters(const glm::vec3 &pos, size_t count);
	std::vector<FighterVessel*> invade(std::vector<FighterVessel*> &vessels);

	// AI needs access to these
	// Next time use event system or something
	enum class VesselType
	{
		Fighter,
		Carrier,
		Supply,
		Discovery
	};
	void createVessel(VesselType type);

	virtual void draw(const Camera &camera) const override final;
	virtual void drawTransparent(const Camera &camera) const override final;

	virtual void onUnselect(Clickable *clickable) override final;
	virtual void onClick(const Ray &ray) override final;
	virtual void onHover(const Ray &ray) override final;
};