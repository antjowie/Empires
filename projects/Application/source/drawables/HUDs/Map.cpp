#include "drawables/HUDs/Map.h"
#include "Shader.h"

#include <glm/glm/ext/matrix_transform.hpp>

Map::Map(const DrawData &drawData):
	HUD(drawData,true),
	m_zoom(1.f),
	m_isFullscreen(false),
	m_planetsVBO(0),
    m_planetIndices(0),
    m_hasGeneratedMap(false),
	m_levelBounds(0),
	m_markersVBO(0),
	m_markersIndices(0),
	m_markersMaxIndices(0),
	m_border(0)
{
	m_zoom = 1.f;
	glGenBuffers(1, &m_planetsVBO);
	setFullscreen(false);

	m_planetProgram = glCreateProgram();
	Shader point;
	point.addShader(GL_VERTEX_SHADER, "resources/shaders/point.vert");
	point.addShader(GL_FRAGMENT_SHADER, "resources/shaders/point.frag");
	if (point.linkProgram(m_planetProgram))
		std::cout << "Loaded map planet shader!\n";
}

Map::~Map()
{
	glDeleteProgram(m_planetProgram);
	glDeleteBuffers(1,&m_planetsVBO);
	glDeleteBuffers(1,&m_markersVBO);
}

Map::Marker Map::createMarker(const Drawable &drawable, const glm::vec4 &color, float scale)
{
	float fScale = drawable.scale().x;
	if (scale != 0.f)
		fScale = scale;

	return createMarker(drawable.pos(), color, fScale);
}

Map::Marker Map::createMarker(const glm::vec3 & pos, const glm::vec4 &color, float scale)
{
	Marker data;

	data.pos[0] = pos[0] * m_levelBounds[0];
	data.pos[1] = -pos[2] * m_levelBounds[1]; // Flip it to go forward when player goes forward
	data.pos[2] = -0.1f;

	for (int i = 0; i < 4; i++)
	{
		data.col[i] = color[i];
	}

	// 1.25f is an arbitrary value that just works
	// the size of a point is defined in fragments (or pixels). This is basically
	// the mapping from the pixel size to the in game size, but it is mostly trial and error
	glm::vec3 scaleMod(static_cast<float>(m_pixels) * m_levelBounds.x * scale * 1.25f);
	if (!m_isFullscreen)
		scaleMod *= this->scale().x;
	else
		scaleMod *= m_zoom;

	data.s = scaleMod.x;

	return data;
}

void Map::generateMap(const std::vector<std::unique_ptr<Planet>>& planets)
{
	std::vector<Marker> buffer;
	std::vector<Marker> sois;

	// Create list of planet positions and size
	// I'm not sure if I need the negative, since the game is placed in the first
	// quadrant aka all positions are positive
	//float cap[3][2];
	//cap[0][0] = cap[0][1] = planets.front()->pos().x;
	//cap[1][0] = cap[1][1] = planets.front()->pos().y;
	//cap[2][0] = cap[2][1] = planets.front()->pos().z;

	assert(planets[0]->soi() && "SOI should first be generated");
	for (const std::unique_ptr<Planet> &planet : planets)
	{
		sois.push_back(createMarker(*planet->soi(), glm::vec4(planet->soi()->color()) * 2.f));
		buffer.push_back(createMarker(*planet, glm::vec4(1)));
	}

	sois.insert(sois.end(), buffer.begin(), buffer.end());

	// Update planetVBO
	glBindBuffer(GL_ARRAY_BUFFER, m_planetsVBO);

	if (m_hasGeneratedMap)
	{
		assert(false && "Regenerating map. Why??");
		glBufferSubData(GL_ARRAY_BUFFER, 0, sois.size() * sizeof(Marker), sois.data());
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, sois.size() * sizeof(Marker), sois.data(), GL_STATIC_DRAW);
	}
	m_planetIndices = sois.size();
	m_hasGeneratedMap = true;
}

void Map::generateMarkers(GLuint maxMarkers)
{
	m_markersMaxIndices = maxMarkers;
	glGenBuffers(1, &m_markersVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_markersVBO);
	glBufferData(GL_ARRAY_BUFFER, maxMarkers * sizeof(Marker), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Map::updatePlanetsSOI(const std::vector<std::unique_ptr<Planet>>& planets)
{
	std::vector<Marker> sois;
	for (const std::unique_ptr<Planet> &planet : planets)
	{
		glm::vec4 color(glm::vec4(planet->soi()->color()));
		if (planet->state() != Planet::State::Colonized && planet->state() != Planet::State::Influenced)
			color.a = 0.f;

		sois.push_back(createMarker(*planet->soi(), color));
	}

	for (const std::unique_ptr<Planet> &planet : planets)
	{
		sois.push_back(createMarker(*planet,glm::vec4(planet->color(),1)));
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_planetsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sois.size() * sizeof(Marker), sois.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Map::setLevelBounds(const glm::vec2 & levelBounds)
{
	m_levelBounds = 1.f / levelBounds;
}

void Map::setViewportHeight(const unsigned pixels)
{
	// Hardware limitations
	// Sizeof point is guranteed to be at least 1. On the Pi it looks around 400
	m_pixels = pixels;
}

void Map::addMarker(const Marker & marker)
{
	m_markers.push_back(marker);
}

void Map::displayMarkers()
{
	// Create a new buffer for OpenGL to load
	assert(m_markers.size() <= m_markersMaxIndices && "Too many markers are being drawn");

	m_markersIndices = m_markers.size();
	glBindBuffer(GL_ARRAY_BUFFER, m_markersVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_markersIndices * sizeof(Marker), m_markers.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_markers.clear();
}

void Map::setFocus(const glm::vec2 & focus)
{
	m_focus = focus * m_levelBounds;
}

void Map::setFullscreen(bool isFullscreen)
{
	if (!isFullscreen)
	{
		setPos(glm::vec3(-0.75f, -0.75f, 0));
		setScale(0.25f);
		setColor(glm::vec4(glm::vec3(0.25f), 0.6f));
		m_isFullscreen = false;
	}
	else
	{
		setPos(glm::vec3(0));
		setScale(1.f);
		setColor(glm::vec4(glm::vec3(0.25f), 1.0f));
		m_isFullscreen = true;
	}
	move(glm::vec3(0, 0, -0.1f));
}

void Map::zoom(const float constant)
{
	m_zoom *= constant;
}

void Map::setZoom(const float constant)
{
	m_zoom = constant;
}

void Map::drawTransparent(const Camera & camera) const
{
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	HUD::drawTransparent(camera);
	
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);

	// I have to update the blend equation to GL_MAX but opengl es 2.0 doesn't support that
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ZERO, GL_ONE);

	glUseProgram(m_planetProgram);
	const GLint mvpLoc = glGetUniformLocation(m_planetProgram, "mvp");

	//glm::vec3 scaleModifier(scale());
	//if (scale() == glm::vec3(1.f))
	//	scaleModifier = glm::vec3(m_zoom);

	glm::vec3 scaleMod(1.f);
	if (!m_isFullscreen)
		scaleMod *= this->scale().x;
	else
		scaleMod *= m_zoom;

	glm::mat4 mvp(1);
	const glm::vec3 translation = pos() + glm::vec3(-m_focus.x,m_focus.y,0) * scaleMod.x;
	mvp = camera.orthogonal();
	mvp = glm::translate(mvp, translation);
	mvp = glm::scale(mvp, glm::vec3(scaleMod.x));

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);
	
	const GLint posLoc = glGetAttribLocation(m_planetProgram, "aVerPos");
	const GLint colLoc = glGetAttribLocation(m_planetProgram, "aTexPos");
	const GLint scaleLoc = glGetAttribLocation(m_planetProgram, "aScale");

	// Draw the planets
	glBindBuffer(GL_ARRAY_BUFFER, m_planetsVBO);

	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Marker), 0);
	glVertexAttribPointer(colLoc, 4, GL_FLOAT, GL_TRUE, sizeof(Marker), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(scaleLoc, 1, GL_FLOAT, GL_FALSE, sizeof(Marker), (GLvoid*)(7 * sizeof(GLfloat)));
	
	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(colLoc);
	glEnableVertexAttribArray(scaleLoc);

	glDrawArrays(GL_POINTS, 0, m_planetIndices);

	// Draw markers
	glBindBuffer(GL_ARRAY_BUFFER, m_markersVBO);
	
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Marker), 0);
	glVertexAttribPointer(colLoc, 4, GL_FLOAT, GL_TRUE, sizeof(Marker), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(scaleLoc, 1, GL_FLOAT, GL_FALSE, sizeof(Marker), (GLvoid*)(7 * sizeof(GLfloat)));

	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(colLoc);
	glEnableVertexAttribArray(scaleLoc);

	glDrawArrays(GL_POINTS, 0, m_markersIndices);

	// By adding this we make sure that other things are still drawn
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
}

glm::mat4 Map::toWorld() const
{
	return HUD::toWorld();
}