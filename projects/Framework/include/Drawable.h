#pragma once
#include "Camera.h"

#include <GLES2/gl2.h>
#undef countof
#include <glm/glm/glm.hpp>


class DrawableFactory;

struct DrawData
{
public:
	GLuint vbo;
	GLuint ebo;
	GLsizei indicesCount;
	GLuint textureID;
	GLuint program;

	GLuint posLoc;
	GLuint texLoc;
	GLuint samLoc;
};

class Drawable
{
private:
	glm::vec3 m_pos;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

protected:
	const DrawData &m_drawData;
	glm::vec3 m_color;

public:
	Drawable(const DrawData &drawData); // A private constructor gives trouble creating unique ptrs for some reason
	virtual ~Drawable();

	void move(const glm::vec3 &pos);
	void setPos(const glm::vec3 &pos);
	void rotate(const glm::vec3 &degrees);
	void setRotation(const glm::vec3 &degrees);
	void setScale(const glm::vec3 &scale);
	void setScale(float scale);

	const glm::vec3 &pos() const;
	const glm::vec3 &rotation() const;
	const glm::vec3 &scale() const;

	virtual void draw(const Camera &camera) const;
	virtual void drawTransparent(const Camera &camera) const;
	//virtual bool onScreen(const glm::mat4 &projection) const;
	virtual glm::mat4 toWorld() const;
};