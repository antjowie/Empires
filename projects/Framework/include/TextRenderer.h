#pragma once

#include "Camera.h"

#include <glm/glm/vec2.hpp>
#include <glm/glm/vec3.hpp>
#include <glm/glm/vec4.hpp>

#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Text
{
public:
	std::string text;
	glm::vec3 pos;
	bool centeredOrigin;
	bool orthogonal;
	glm::vec4 color;
	float scale;

	Text(
		const std::string &text, 
		const glm::vec3 &pos, 
		bool centeredOrigin = false,
		bool orthogonal = false,
		const glm::vec4 &color = glm::vec4(1.f), 
		float scale = 1.f);
	Text();
};

class TextRenderer
{
private:
	struct Character
	{
	public:
		unsigned texture;
		glm::vec2 size;
		glm::vec2 bearing;
		unsigned advance;
	};

	unsigned m_vbo;
	unsigned m_program;

	int m_verPos;
	int m_texPos;

	int m_colorPos;
	int m_projPos;
	int m_samplPos;

	FT_Face m_face;
	FT_Library m_library;
	std::vector<Character> m_characters;
	std::vector<Text> m_toDraw;

	void clear();

public:
	~TextRenderer();

	bool init();

	bool loadFont(const char* path);
	void drawText(const Text &text);
	void render(const Camera &camera);
};