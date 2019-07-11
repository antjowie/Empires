#pragma once
#include "drawables/HUDs/HUD.h"
#include "drawables/Planet.h"

class Map : public HUD
{
public:
	struct Marker
	{
		float pos[3];
		float col[4];
		float s;
	};


private:
	glm::vec2 m_focus;
	float m_zoom;
	bool m_isFullscreen;

	GLuint m_planetsVBO;
	GLuint m_planetIndices;
	bool m_hasGeneratedMap;

	glm::vec2 m_levelBounds;

	unsigned m_pixels;
	GLuint m_planetProgram;

	std::vector<Marker> m_markers;

	GLuint m_markersVBO;
	GLuint m_markersIndices;
	GLuint m_markersMaxIndices;

	float m_border;

public:
	Map(const DrawData &drawData);
	virtual ~Map() override final;

	Marker createMarker(const Drawable &drawable, const glm::vec4 &color, float scale = 0.f);
	Marker createMarker(const glm::vec3 & pos, const glm::vec4 &color, float scale);

	void generateMap(const std::vector<std::unique_ptr<Planet>> &planets);
	void generateMarkers(GLuint maxMarkers);

	void updatePlanetsSOI(const std::vector<std::unique_ptr<Planet>> &planets);

	void setLevelBounds(const glm::vec2 &levelBounds);
	void setViewportHeight(const unsigned pixels);

	void addMarker(const Marker &marker);
	// Call this before the draw call
	void displayMarkers();

	void setFocus(const glm::vec2 &focus);
	void setFullscreen(bool isFullscreen);

	void zoom(const float constant);
	void setZoom(const float constant);

	virtual void drawTransparent(const Camera &camera) const override final;
	virtual glm::mat4 toWorld() const override final;
};