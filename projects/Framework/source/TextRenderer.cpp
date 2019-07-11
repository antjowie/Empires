#include "TextRenderer.h"
#include "Shader.h"

#include <GLES2/gl2.h>
#include <glm/glm/gtc/type_ptr.hpp>

Text::Text(const std::string & text, const glm::vec3 & pos, bool centeredOrigin, bool orthogonal, const glm::vec4 & color, float scale):
	text(text),
	pos(pos),
	centeredOrigin(centeredOrigin),
	orthogonal(orthogonal),
	color(color),
	scale(scale / 64.f)
{
}

Text::Text() :
	Text("No text", glm::vec3(0))
{
}

void TextRenderer::clear()
{
	for (size_t i = 0; i < m_characters.size(); i++)
		glDeleteTextures(1, &m_characters[i].texture);
	m_characters.clear();
}

TextRenderer::~TextRenderer()
{
	FT_Done_FreeType(m_library);
	clear();
}

bool TextRenderer::init()
{
	FT_Error error = FT_Init_FreeType(&m_library);
	if (error != FT_Err_Ok) 
	{
		printf("Couldn't initialize freetype, wut?\n");
		return false;
	}

	// Create the VBO
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Load the shader
	m_program = glCreateProgram();
	Shader shader;
	shader.addShader(GL_VERTEX_SHADER,   "resources/shaders/text.vert");
	shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/text.frag");
	shader.linkProgram(m_program);

	glUseProgram(m_program);
	m_verPos = glGetAttribLocation(m_program, "aVer");
	m_texPos = glGetAttribLocation(m_program, "aTex");
	
	m_colorPos = glGetUniformLocation(m_program, "textColor");
	m_projPos = glGetUniformLocation(m_program, "projection");
	m_samplPos = glGetUniformLocation(m_program, "text");
	
	// Texture is grey (1 byte only) but OpenGL expects 4 bytes which can cause jittering
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	return true;
}

bool TextRenderer::loadFont(const char * path)
{
	// Took some code from here
	// https://learnopengl.com/In-Practice/Text-Rendering
	if (FT_New_Face(m_library, path, 0, &m_face) != FT_Err_Ok)
	{
		printf("Couldn't load face\n");
		return false;
	}
	
	if (FT_Set_Pixel_Sizes(m_face, 0, 48))
	{
		printf("Couldn't set pixel size\n");
		return false;
	}

	// Clear old characters
	clear();

	// Just load ASCII
	for (unsigned char i = 0; i < 128; i++)
	{
		if (FT_Load_Char(m_face, i, FT_LOAD_RENDER))
		{
			printf("Couldn't load glyph %i so lets crash the application\n", i);
			clear();
			return false;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			m_face->glyph->bitmap.buffer
		);
		assert(glGetError() == GL_NO_ERROR);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		m_characters.push_back(Character{
			texture,
			glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
			glm::ivec2(m_face->glyph->bitmap_left,  m_face->glyph->bitmap_top),
			static_cast<unsigned>(m_face->glyph->advance.x)
			});
	}
	printf("Loaded glyphs\n");
	glBindTexture(GL_TEXTURE_2D, 0);
	// Clear the resources
	FT_Done_Face(m_face);
	return true;
}

void TextRenderer::drawText(const Text & text)
{
	m_toDraw.push_back(text);
}

void TextRenderer::render(const Camera &camera)
{
	glUseProgram(m_program);

	for (Text &text : m_toDraw)
	{
		if (text.orthogonal)
			glUniformMatrix4fv(m_projPos, 1, GL_FALSE, glm::value_ptr(camera.orthogonal()));
		else
			glUniformMatrix4fv(m_projPos, 1, GL_FALSE, glm::value_ptr(camera.perspective() * camera.view()));

		glUniform3f(m_colorPos, text.color.x, text.color.y, text.color.z);
		glUniform3fv(m_colorPos, 1, glm::value_ptr(text.color));
		
		float x = text.pos.x;
		if (text.centeredOrigin)
		{
			float totalWidth = 0;
			for (std::string::const_iterator c = text.text.begin(); c != text.text.end(); c++)
			{
				const Character &ch = m_characters[*c];

				totalWidth += float(ch.advance >> 6) * text.scale;
			}
			x -= totalWidth * 0.5f;
		}

		for (std::string::const_iterator c = text.text.begin(); c != text.text.end(); c++)
		{
			const Character &ch = m_characters[*c];
			
 			const GLfloat xpos = x + ch.bearing.x * text.scale;
			const GLfloat ypos = text.pos.y - (ch.size.y - ch.bearing.y) * text.scale;
			
			const GLfloat w = ch.size.x * text.scale;
			const GLfloat h = ch.size.y * text.scale;
			// Update VBO for each character

			const GLfloat vertices[6][5] = {
				{ xpos,     ypos + h, text.pos.z,  0.0, 0.0 },
				{ xpos,     ypos,     text.pos.z,  0.0, 1.0 },
				{ xpos + w, ypos,     text.pos.z,  1.0, 1.0 },
									  
				{ xpos,     ypos + h, text.pos.z,  0.0, 0.0 },
				{ xpos + w, ypos,     text.pos.z,  1.0, 1.0 },
				{ xpos + w, ypos + h, text.pos.z,  1.0, 0.0 }
			};

			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.texture);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			
			glVertexAttribPointer(m_verPos, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5.f, 0);
			glEnableVertexAttribArray(m_verPos);
			
			glVertexAttribPointer(m_texPos, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5.f, (GLvoid*)(sizeof(GLfloat) * 3));
			glEnableVertexAttribArray(m_texPos);
	
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += float(ch.advance >> 6) * text.scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
			
		}
	}
	
	m_toDraw.clear();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}